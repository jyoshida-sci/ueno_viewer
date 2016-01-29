#include "nagara_viewer.h"

#include "qsettings.h"

#include "qlineedit.h"
#include "qpushbutton.h"
#include "qlayout.h"
#include "qlabel.h"
#include "qstring.h"
#include "qlcdnumber.h"
#include "qcheckbox.h"

#include <QMouseEvent>

#include "qfiledialog.h"
#include "qdir.h"

nagara_viewer::nagara_viewer(QWidget *parent)
	: QWidget(parent)
{
//	ui.setupUi(this);

	appsettings = new QSettings("nagara_viewer.ini", QSettings::IniFormat);


	QVBoxLayout* lay = new QVBoxLayout();

	//file path
	QHBoxLayout *lay_file = new QHBoxLayout();
	line_file = new QLineEdit(QString("no file yet"));
	but_file = new QPushButton(tr("&Read file"));
	lcd_ipict = new QLCDNumber();
	chk_halfsize = new QCheckBox("halfsize");
	chk_halfsize->setCheckState(Qt::Checked);
	lay_file->addWidget(line_file);
	lay_file->addWidget(but_file);
	lay_file->addWidget(lcd_ipict);
	lay_file->addWidget(chk_halfsize);
	lay->addLayout(lay_file);


	//image
	but_up = new QPushButton(tr("&Up"));
	lay->addWidget(but_up);
	lab_img1 = new QLabel();
	lay->addWidget(lab_img1);
	//lab_img2 = new QLabel();
	//lay->addWidget(lab_img2);
	//lab_img3 = new QLabel();
	//lay->addWidget(lab_img3);

	but_down = new QPushButton(tr("&Down"));
	lay->addWidget(but_down);

	//layout
	setLayout(lay);


	//connect signal-slot
	connect(but_file, SIGNAL(clicked()), this, SLOT(loadImg()));
	connect(but_up, SIGNAL(clicked()), this, SLOT(ImgUp()));
	connect(but_down, SIGNAL(clicked()), this, SLOT(ImgDown()));
	connect(this, SIGNAL(wheelEvent(QWheelEvent*)), this, SLOT(changeLayer(QWheelEvent*)));

	but_up->setDisabled(true);
	but_down->setDisabled(true);

}

nagara_viewer::~nagara_viewer()
{
	Init();
}



bool nagara_viewer::Init()
{
	for (int i = 0; i<vbd.size(); i++)	free(vbd[i]);
	vbd.clear();
	return true;
}



bool nagara_viewer::updateImg(int v, int i){//v: not used yet

	cv::Mat mat1(he, wi, CV_8U);
	for (int by = 0; by<bpp; by++){
		char c = vbd[i][by];
		for (int j = 0; j<8; j++){
			mat1.data[by * 8 + j] = (c >> j & 1) * 255;
		}
	}

	//cv::Mat mat2(he, wi, CV_8U);
	//for (int by = 0; by<bpp; by++){
	//	char c = vbd[i + nlayer][by];
	//	for (int j = 0; j<8; j++){
	//		mat2.data[by * 8 + j] = (c >> j & 1) * 255;
	//	}
	//}

	//cv::Mat mat3(he, wi, CV_8U);
	//for (int by = 0; by<bpp; by++){
	//	char c = vbd[i + nlayer * 2][by];
	//	for (int j = 0; j<8; j++){
	//		mat3.data[by * 8 + j] = (c >> j & 1) * 255;
	//	}
	//}

	//cv::imwrite("1.png",mat1);
	//cv::imwrite("2.png",mat2);
	//cv::imwrite("3.png",mat3);

	//cv::resize(mat1, mat1, cv::Size(), 0.5, 0.5);
	//cv::resize(mat2, mat2, cv::Size(), 0.5, 0.5);
	if (chk_halfsize->isChecked()){
		cv::pyrDown(mat1, mat1);
		//cv::pyrDown(mat2, mat2);
		//cv::pyrDown(mat3, mat3);
	}

	QImage image1(mat1.data,
		mat1.cols,
		mat1.rows,
		//QImage::Format_RGB32);
		QImage::Format_Indexed8);
	image1 = image1.convertToFormat(QImage::Format_RGB32);
	lab_img1->setPixmap(QPixmap::fromImage(image1));

	//QImage image2(mat2.data,
	//	mat2.cols,
	//	mat2.rows,
	//	//QImage::Format_RGB32);
	//	QImage::Format_Indexed8);
	//image2 = image2.convertToFormat(QImage::Format_RGB32);
	//lab_img2->setPixmap(QPixmap::fromImage(image2));

	//QImage image3(mat3.data,
	//	mat3.cols,
	//	mat3.rows,
	//	//QImage::Format_RGB32);
	//	QImage::Format_Indexed8);
	//image3 = image3.convertToFormat(QImage::Format_RGB32);
	//lab_img3->setPixmap(QPixmap::fromImage(image3));

	lcd_ipict->display(i);
	return true;
}


bool nagara_viewer::changeLayer(QWheelEvent *event)
{
	if (event->delta()<0){
		ipict++;
		if (ipict == vbd.size() ) ipict = vbd.size()-1;
		if (ipict != 0) but_up->setEnabled(true);
		if (ipict == vbd.size()-1 ) but_down->setDisabled(true);
		if (!updateImg(1, ipict)) return false;
	}
	else{
		ipict--;
		if (ipict == -1)ipict = 0;
		if (ipict == 0) but_up->setDisabled(true);
		if (ipict != vbd.size() -1 ) but_down->setEnabled(true);
		if (!updateImg(1, ipict)) return false;
	}
	return true;

}


bool nagara_viewer::ImgUp()
{
	ipict--;
	if (!updateImg(1, ipict)) return false;
	if (ipict == 0) but_up->setDisabled(true);
	if (ipict != vbd.size()-1 ) but_down->setEnabled(true);
	return true;
}

bool nagara_viewer::ImgDown()
{
	ipict++;
	if (!updateImg(1, ipict)) return false;
	if (ipict != 0) but_up->setEnabled(true);
	if (ipict == vbd.size()-1 ) but_down->setDisabled(true);
	return true;
}



bool nagara_viewer::loadImg(){

	Init();

	QString fileName = QFileDialog::getOpenFileName(
		this,
		tr("Open files"),
		QDir::homePath(),
		tr("DAT Files (*.dat);;All Files (*)"));
	if (fileName.isEmpty())return false;

	QFile file( (const char*) fileName.toLatin1().data() );
	if (!file.open(QIODevice::ReadOnly)) return false;

	QDataStream in(&file);
	while (!in.atEnd()) {
		char* b = (char *)malloc(bpp);
		if (b == NULL)return false;
		in.readRawData(b, bpp);
		vbd.push_back(b);
	}

	ipict = 0;
	if (!updateImg(1, ipict)) return false;
	but_down->setEnabled(true);

	line_file->setText(QString("%1").arg(fileName));

	return true;
}
