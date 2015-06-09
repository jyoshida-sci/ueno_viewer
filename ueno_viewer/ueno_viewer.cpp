#include "ueno_viewer.h"

#include "qsettings.h"

#include "qlineedit.h"
#include "qpushbutton.h"
#include "qlayout.h"
#include "qlabel.h"
#include "qstring.h"
#include "qlcdnumber.h"
#include "qcheckbox.h"
#include "qtextedit.h"
#include "qtextstream.h"
#include "qslider.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "qdebug.h"

#include "qfiledialog.h"
#include "qdir.h"
#include "qmenu.h"
#include <QMouseEvent>

#include "QClickableLabel.h"

#include <stdio.h>
#include <stdlib.h>
#include <string>

//#include <libClustring\Clustring2D.h>
//#include <libClustring\Clustring3D.h>
//#include <libFitting\GaussianFitting.h>


ueno_viewer::ueno_viewer(QWidget *parent)
	: QWidget(parent)
{
	appsettings = new QSettings("ueno_viewer.ini", QSettings::IniFormat);

	QGridLayout* lay = new QGridLayout();


	//image
	but_up = new QPushButton(tr("&Prev"));
	lay->addWidget(but_up, 0, 0, 1, 2);
	lab_img_o = new QLabel();
	lay->addWidget(lab_img_o, 1, 0);
	lab_img_f = new QLabel();
	lay->addWidget(lab_img_f, 2, 0);
	lab_img = new QClickableLabel();
	lab_img->setMouseTracking(true);
	lay->addWidget(lab_img, 1, 1, 2, 1);
	but_down = new QPushButton(tr("&Next"));
	lay->addWidget(but_down, 3, 0, 1, 2);


	//click info
	QHBoxLayout *lay_click = new QHBoxLayout();
	lab_pix = new QLabel();
	lab_pix->setFont(QFont("Arial",10));
	lay_click->addWidget(lab_pix);
	lab_pix_cl = new QLabel();
	lab_pix_cl->setFont(QFont("Arial",10));
	lay_click->addWidget(lab_pix_cl);
	lab_pix_dr = new QLabel();
	lab_pix_dr->setFont(QFont("Arial",10));
	lay_click->addWidget(lab_pix_dr);
	lay->addLayout(lay_click, 4, 0, 1, 2);

	QHBoxLayout *lay_stage = new QHBoxLayout();
	lab_stg = new QLabel();
	lab_stg->setFont(QFont("Arial", 10));
	lay_stage->addWidget(lab_stg);
	lab_stg_cl = new QLabel();
	lab_stg_cl->setFont(QFont("Arial", 10));
	lay_stage->addWidget(lab_stg_cl);
	lab_stg_dr = new QLabel();
	lab_stg_dr->setFont(QFont("Arial", 10));
	lay_stage->addWidget(lab_stg_dr);
	lay->addLayout(lay_stage, 5, 0, 1, 2);



	//file path
	QHBoxLayout *lay_file = new QHBoxLayout();
	line_file = new QLineEdit(QString("no file yet"));
	but_file = new QPushButton(tr("&Read file"));
	lcd_img = new QLCDNumber(); 
	chk_filterd = new QCheckBox("filterd");
	//chk_filterd->setCheckState(Qt::Checked);
	lay_file->addWidget(line_file);
	lay_file->addWidget(but_file);
	lay_file->addWidget(lcd_img);
	lay_file->addWidget(chk_filterd);
	lay->addLayout(lay_file, 6, 0, 1, 2);

	sli_z = new QSlider();
	lay->addWidget(sli_z, 0, 2, 6, 1);

	//texteditarea
	txt_clicked = new QTextEdit(); 
	txt_clicked->setFixedWidth(170);
	lay->addWidget(txt_clicked, 0, 3, 5, 1);
	but_writxt = new QPushButton(tr("&Write File"));
	lay->addWidget(but_writxt, 5, 3, 1, 1);
	//but_make = new QPushButton(tr("&Make pict"));
	//lay->addWidget(but_make, 4, 2, 1, 1);


	//layout
	setLayout(lay);


	//connect signal-slot
	connect(but_file, SIGNAL(clicked()), this, SLOT(loadImg()));
	connect(but_up, SIGNAL(clicked()), this, SLOT(ImgUp()));
	connect(but_down, SIGNAL(clicked()), this, SLOT(ImgDown()));

	connect(but_writxt, SIGNAL(clicked()), this, SLOT(writeTxtFile()));
	connect(this,SIGNAL(wheelEvent(QWheelEvent*)),this,SLOT(changeLayer(QWheelEvent*)));
	connect(lab_img, SIGNAL(mousePressed(QMouseEvent*)),this,SLOT(displayMousePos(QMouseEvent*)));
	connect(lab_img, SIGNAL(mousePressed(QMouseEvent*)),this,SLOT(labMouseClicked(QMouseEvent*)));
	connect(lab_img, SIGNAL(mousePressed(QMouseEvent*)),this,SLOT(showContextMenu(QMouseEvent*)));
	connect(lab_img, SIGNAL(mouseMoved(QMouseEvent*)),this,SLOT(labMouseMoved(QMouseEvent*)));
	connect(sli_z, SIGNAL(valueChanged(int)), this, SLOT(changeToNthLayer(int)));

	but_up->setDisabled(true);
	but_down->setDisabled(true);
	sli_z->setDisabled(true);

}

ueno_viewer::~ueno_viewer()
{
	Init();
}


void ueno_viewer::Init()
{
	vomat.clear();
	vfmat.clear();
	//vc2.clear();
	//vc3.clear();
}


bool ueno_viewer::updateImg(int i){

	cv::Mat mat = vomat[i];

	if(chk_filterd->isChecked()){
		mat = vfmat[i];
	}

	QImage image(mat.data, 
					mat.cols, 
					mat.rows, 
					QImage::Format_RGB888);
					//QImage::Format_Indexed8);
	image = image.convertToFormat(QImage::Format_RGB32);
	lab_img->setPixmap(QPixmap::fromImage(image));

	return true;
}


bool ueno_viewer::changeLayer(QWheelEvent *event)
{
	if(event->delta()<0){
		ipict++;
		if(ipict==vomat.size())ipict=vomat.size()-1;
		if(ipict!=0) but_up->setEnabled(true);
		if(ipict==vomat.size()-1) but_down->setDisabled(true);
		lcd_img->display(ipict);
		if ( !updateImg(ipict) ) return false;
	 }else{
		ipict--;
		if(ipict==-1)ipict=0;
		if(ipict==0) but_up->setDisabled(true);
		if(ipict!=vomat.size()-1) but_down->setEnabled(true);
		lcd_img->display(ipict);
		if ( !updateImg(ipict) ) return false;
	}
	sli_z->setValue(ipict);
	return true;
}


bool ueno_viewer::imgUp()
{
	ipict--;
	if(ipict==-1)ipict=0;
	if(ipict==0) but_up->setDisabled(true);
	if(ipict!=vomat.size()-1) but_down->setEnabled(true);
	lcd_img->display(ipict);
	if ( !updateImg(ipict) ) return false;
	sli_z->setValue(ipict);
	return true;
}

bool ueno_viewer::imgDown()
{
	ipict++;
	if(ipict==vomat.size())ipict=vomat.size()-1;
	if(ipict!=0) but_up->setEnabled(true);
	if(ipict==vomat.size()-1) but_down->setDisabled(true);
	lcd_img->display(ipict);
	if ( !updateImg(ipict) ) return false;
	sli_z->setValue(ipict);
	return true;	
}

void ueno_viewer::changeToNthLayer(int i){
	ipict = i;
	if (ipict == 0){
		but_up->setDisabled(true);
	}
	else{
		but_up->setEnabled(true);
	}

	if (ipict == vomat.size() - 1){
		but_down->setDisabled(true);
	}
	else{
		but_down->setEnabled(true);
	}
	lcd_img->display(ipict);
	updateImg(ipict);
}


bool ueno_viewer::loadImg(){

	Init();
	
	QDir dir = appsettings->value("readdir").toString();
	qDebug() << dir.absolutePath();

	//dialog
	QString fileName = QFileDialog::getOpenFileName(
		this,
		tr("Open files"),
		dir.absolutePath(),
		tr("JSON Files (*.json);;All Files (*)"));
	if(fileName.isEmpty())return false;

	//dir path
	dir = QFileInfo(fileName).absoluteDir();
	appsettings->setValue("readdir", dir.absolutePath());

	//file open
	QFile file((const char *)fileName.toLatin1().data());
	if (!file.open(QIODevice::ReadOnly)) return false;

	//json file
	QString content = file.readAll();
	file.close();

	QJsonDocument jsondoc = QJsonDocument::fromJson(content.toUtf8());
	QJsonObject jsonobj = jsondoc.object();
	QJsonArray array_images = jsonobj["Images"].toArray();

	wi = jsonobj["ImageType"].toObject()["Width"].toInt();
	he = jsonobj["ImageType"].toObject()["Height"].toInt();
	Sh = jsonobj["Sh"].toDouble();
	um_px = jsonobj["Interval"].toArray().at(0).toDouble();
	um_py = jsonobj["Interval"].toArray().at(1).toDouble();
	um_pz = jsonobj["Interval"].toArray().at(2).toDouble();
	viewx = jsonobj["InitialPos"].toArray().at(0).toDouble();
	viewy = jsonobj["InitialPos"].toArray().at(1).toDouble();
	viewz = jsonobj["InitialPos"].toArray().at(2).toDouble();

	//read img files
	for(int p=0; p<array_images.count(); p++){
		std::string dirpath = dir.absolutePath().toLocal8Bit().constData();
		std::string filename = array_images.at(p).toObject()["Path"].toString().toLocal8Bit().constData();
		std::string filefullpath = dirpath + "/" + filename;

		cv::Mat mat1 = cv::imread(filefullpath, 0);//gray scale
		cv::cvtColor( mat1, mat1, CV_GRAY2BGR );
		vomat.push_back(mat1);

		cv::Mat mat2 = mat1.clone();	
		cv::GaussianBlur(mat2, mat2, cv::Size(3, 3), -1);
		cv::Mat smooth_img = mat2.clone();
		cv::GaussianBlur(smooth_img, smooth_img, cv::Size(31, 31), -1);
		cv::subtract(smooth_img, mat2, mat2);

		//double min,max;
		//cv::minMaxLoc( mat2,&min,&max,0,0);
		//cv::convertScaleAbs( mat2,  mat2, 255/(max-min), -255*min/(max-min));

		cv::threshold(mat2, mat2, 25, 255, CV_THRESH_TOZERO);

		vfmat.push_back(mat2);
	}




	//•À—ñˆ—‚Å‚Íˆ—‚Ì‡”Ô‚Í‚»‚Ì“s“x•Ï‚í‚éB‡”Ô‚ª•Ï‚í‚Á‚Ä‚à–â‘è‚È‚¢ˆ—‚ð‚±‚±‚Å‚â‚Á‚Ä‚¢‚éB
	#ifdef _OPENMP// without_openmp: 25sec / openmp:7.5sec
	#pragma omp parallel for
	#endif
	for(int p=0; p<vomat.size(); p++){

		//Clustring2D c2d;
		//vc2 =  c2d.DoClustring2D(p, vfmat[p], 5.0);
		//vc2.insert(vc2.end(), vc.begin(), vc.end());

		double min,max;
		cv::minMaxLoc( vfmat[p],&min,&max,0,0);
		cv::convertScaleAbs( vfmat[p], vfmat[p], 255/(max-min), -255*min/(max-min));
		//cv::cvtColor(  vfmat[p], vfmat[p], CV_GRAY2BGR );

		//for(int c=0; c<vc.size(); c++){
		//	cv::circle( vfmat[p], cv::Point(vc[c].x,vc[c].y), 1, cv::Scalar(255,0,0));
		//}
	}

	//Clustring3D c3d;
	//vc3 =  c3d.DoClustring3D(vc2, 5.0, 5.0);

	//for(int c=0; c<vc3.size(); c++){
	//	int n = (int)( vc3[c].z < 0.0 ? vc3[c].z-0.5 : vc3[c].z+0.5 );//round
	//	cv::circle(vfmat[n], cv::Point(vc3[c].x,vc3[c].y), 1, cv::Scalar(255,0,0));
	//}
	
	ipict=0;
	if ( !updateImg(ipict) ) return false;
	but_down->setEnabled(true);
	sli_z->setEnabled(true);
	sli_z->setMaximum(vfmat.size()-1);

	line_file->setText(QString("%1").arg(fileName));


	//sub-display
	QImage image_o(	9*24, 
					9*24, 
					QImage::Format_RGB888);
	image_o = image_o.convertToFormat(QImage::Format_RGB32);
	lab_img_o->setPixmap(QPixmap::fromImage(image_o));

	
	//sub-display
	QImage image_f(	9*24, 
					9*24, 
					QImage::Format_RGB888);
	image_f = image_f.convertToFormat(QImage::Format_RGB32);
	lab_img_f->setPixmap(QPixmap::fromImage(image_f));

	return true;
}


void ueno_viewer::displayMousePos(QMouseEvent* e){
	int px = e->x();
	int py = e->y();

	QString str = txt_clicked->toPlainText();
	str += QString("%1  %2  %3\n").arg(px).arg(py).arg(ipict);
	txt_clicked->setText(str);	
}



void ueno_viewer::labMouseClicked(QMouseEvent* e){

	if(e->buttons() & Qt::LeftButton){
		lx = e->x();
		ly = e->y();
		lz = ipict;
		lab_pix_cl->setText(QString("click: %1, %2, %3").arg(lx, 5, 10).arg(ly, 5, 10).arg(lz, 3, 7));

		double stage[3];
		stage[0] = -(lx - wi / 2)*um_px;
		stage[1] = (ly - he / 2)*um_py;
		stage[2] = ipict * um_pz;
		lab_stg_cl->setText(QString("click: %1, %2, %3").arg(viewx + stage[0]).arg(viewy + stage[1]).arg(viewz + stage[2]));

	}

}


void ueno_viewer::labMouseMoved(QMouseEvent* e){

	int cx = e->x();
	int cy = e->y();
	int cz = ipict;
	lab_pix->setText(QString("current pos: %1, %2, %3").arg(cx, 5, 10).arg(cy, 5, 10).arg(cz, 3, 7));

	double stage[3];
	stage[0] = -(cx - wi / 2)*um_px;
	stage[1] = (cy - he / 2)*um_py;
	stage[2] = ipict * um_pz;
	lab_stg->setText(QString("current pos: %1, %2, %3").arg(viewx + stage[0]).arg(viewy + stage[1]).arg(viewz + stage[2]));

	if(e->buttons() & Qt::LeftButton){
		int dx = cx - lx;
		int dy = cy - ly;
		int dz = cz - lz;

		lab_pix_dr->setText(QString("drag: %1, %2, %3").arg(dx, 5, 10).arg(dy, 5, 10).arg(dz, 3, 7));
		lab_stg_dr->setText(QString("drag: %1, %2, %3 (%4)").arg(dx*um_px).arg(dy*um_py).arg(dz*um_pz).arg(dz*um_pz*Sh));

	}

	updateSubDisplay(e);

}


void ueno_viewer::updateSubDisplay(QMouseEvent* e){
	if(vomat.size()==0) return;

	int cx = e->x();
	int cy = e->y();

	//update sub-display for original img
	if ((cx-4)>=0 && (cy-4)>=0 &&(cx+4)<=wi  && (cy+4)<=he ) {
		const int cellsize = 24;//should be 4*n;
		cv::Mat mat_o = cv::Mat(9*cellsize, 9*cellsize, CV_8UC3);

		for(int iy=0; iy<9; iy++){
			for(int ix=0; ix<9; ix++){
				int b  = vomat[ipict].data[(wi*(cy-4+iy)+(cx-4+ix))*3];
				cv::rectangle(mat_o,
					cv::Point(ix*cellsize,iy*cellsize),
					cv::Point((ix+1)*cellsize,(iy+1)*cellsize),
					cv::Scalar(b,b,b),
					-1,
					CV_AA);
				char cb[16];
				sprintf(cb,"%d",b);
				cv::putText(mat_o,
					cb,
					cv::Point(ix*cellsize,iy*cellsize+cellsize/2),
					cv::FONT_HERSHEY_SIMPLEX,
					0.3,
					cv::Scalar(255,0,0),
					1,
					4);
			}
		}
		cv::rectangle(mat_o, cv::Point(4*cellsize,4*cellsize), cv::Point(5*cellsize,5*cellsize), cv::Scalar(0,255,0), 1, CV_AA);

		QImage image_o(mat_o.data, 
						mat_o.cols, 
						mat_o.rows, 
						QImage::Format_RGB888);
		image_o = image_o.convertToFormat(QImage::Format_RGB32);
		lab_img_o->setPixmap(QPixmap::fromImage(image_o));




		//update sub-display for filered img
		cv::Mat mat_f = cv::Mat(9*cellsize, 9*cellsize, CV_8UC3);

		for(int iy=0; iy<9; iy++){
			for(int ix=0; ix<9; ix++){
				int b  = vfmat[ipict].data[(wi*(cy-4+iy)+(cx-4+ix))*3];
				cv::rectangle(mat_f,
					cv::Point(ix*cellsize,iy*cellsize),
					cv::Point((ix+1)*cellsize,(iy+1)*cellsize),
					cv::Scalar(b,b,b),
					-1,
					CV_AA);
				char cb[16];
				sprintf(cb,"%d",b);
				cv::putText(mat_f,
					cb,
					cv::Point(ix*cellsize,iy*cellsize+cellsize/2),
					cv::FONT_HERSHEY_SIMPLEX,
					0.3,
					cv::Scalar(255,0,0),
					1,
					4);
			}
		}
		cv::rectangle(mat_f, cv::Point(4*cellsize,4*cellsize), cv::Point(5*cellsize,5*cellsize), cv::Scalar(0,255,0), 1, CV_AA);

		QImage image_f(mat_f.data, 
						mat_f.cols, 
						mat_f.rows, 
						QImage::Format_RGB888);
		image_f = image_f.convertToFormat(QImage::Format_RGB32);
		lab_img_f->setPixmap(QPixmap::fromImage(image_f));


	}
}

void ueno_viewer::showContextMenu(QMouseEvent* e)
{
	//Right-click context menus with Qt
	//http://www.setnode.com/blog/right-click-context-menus-with-qt/

	if(e->buttons() & Qt::RightButton){
		QPoint globalPos = this->mapToGlobal(e->pos());

		QMenu myMenu;
		myMenu.addAction("Get the darkest in 25layers");
		myMenu.addAction("Get the darkest in 50layers");
		myMenu.addAction("Set the start point");
		myMenu.addAction("Set the end point");

		QAction* selectedItem = myMenu.exec(globalPos);
		if (selectedItem){
			if (selectedItem->text() == "Get the darkest in 25layers_UnderConstruction"){
				QString str = txt_clicked->toPlainText();
				str += QString("25252525\n");
				txt_clicked->setText(str);
				getTheDarkestZ(e->x(), e->y(), ipict, 25);
			}else if(selectedItem->text() == "Get the darkest in 50layers_UnderConstruction"){
				QString str = txt_clicked->toPlainText();
				str += QString("50000000\n");
				txt_clicked->setText(str);
				getTheDarkestZ(e->x(), e->y(), ipict, 50);
			}else if (selectedItem->text() == "Set the start point"){

				start_x = e->x();
				start_y = e->y();
				start_z = ipict;

				QString str = txt_clicked->toPlainText();
				str += QString("start pos: %1 %2 %3\n").arg(start_x).arg(start_y).arg(start_z);
				txt_clicked->setText(str);


			}
			else if (selectedItem->text() == "Set the end point"){

				end_x = e->x();
				end_y = e->y();
				end_z = ipict;
				QString str = txt_clicked->toPlainText();
				str += QString("end pos: %1 %2 %3\n").arg(end_x).arg(end_y).arg(end_z);	
				
				double dx = -(end_x - start_x)*um_px;
				double dy = (end_y - start_y)*um_py;
				double dz = (end_z - start_z)*um_pz;
				double range = sqrt(dx*dx + dy*dy +dz*dz*Sh*Sh);
				

				str += QString("Range= %1\n").arg(range);

				
				double phi = atan2(dy, dx) * 180 / M_PI;
				if (dy < 0) phi += 360.0;

				str += QString("Angle phi= %1\n").arg(phi);

				double theta = acos(dz*Sh / range)* 180/ M_PI;
				str += QString("Angle theta= %1\n").arg(theta);

				txt_clicked->setText(str);
			}
			
		}//if (selectedItem)
	}
}

void ueno_viewer::writeTxtFile(){

	QDir dir = appsettings->value("writedir").toString();
	qDebug() << dir.absolutePath();
	//dialog
	QString selFilter = "All files (*.txt)";
	QString fileName = QFileDialog::getSaveFileName(
		this,
		"Save file",
		dir.absolutePath(),
		"Text files (*.txt);;All files (*.*)",
		&selFilter);
	
	dir = QFileInfo(fileName).absoluteDir();
	appsettings->setValue("writedir", dir.absolutePath());

	// write text file 
	QFile file(fileName);
		if (file.open(QIODevice::WriteOnly)) {
			QTextStream stream(&file);
			QString str = txt_clicked->toPlainText();
			stream << str;
		}

}

void ueno_viewer::getTheDarkestZ(int x, int y, int z,  int range)
{
	//std::vector<double> v_z_pos;
	//std::vector<double> v_z_bright;

	//for(int i=0; i<vomat.size();i++ ){
	//	v_z_pos.push_back( i );
	//	v_z_bright.push_back( vomat[i].data[(sony_wi*y+x)*3] );
	//}

	//GaussianFitting gf;
	//FittingResult myfr = gf.Fitting( v_z_pos, v_z_bright, z, range, true);

	//QString str = txt_clicked->toPlainText();
	//str += QString("height= %1\n").arg(myfr.height);
	//str += QString("mean= %1\n").arg(myfr.mean);
	//str += QString("sigma= %1\n").arg(myfr.sigma);
	//str += QString("intercept= %1\n").arg(myfr.intercept);
	//str += QString("slope= %1\n").arg(myfr.slope);
	//str += QString("chi2= %1\n").arg(myfr.chi2);
	//txt_clicked->setText(str);
}
