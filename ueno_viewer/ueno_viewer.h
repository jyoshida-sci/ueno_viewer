
#ifndef UENO_VIEWER_H
#define UENO_VIEWER_H

#include <QtWidgets/QWidget>
#include "ui_ueno_viewer.h"

#include "..\opencv2411.h"


class QSettings;
class QLabel;
class QPixmap;
class QLineEdit;
class QPushButton;
class QLCDNumber;
class QCheckBox;
class QTextEdit;
class QClickableLabel;
class QSlider;


class ueno_viewer : public QWidget
{
	Q_OBJECT

public:
	ueno_viewer(QWidget *parent = 0);
	~ueno_viewer();

signals:
	void wheelEvent(QWheelEvent *);
	//void mousePressEvent(QMouseEvent *);


private slots:
	bool updateImg(int i);
	bool imgUp();
	bool imgDown();
	bool loadImg();
	bool changeLayer(QWheelEvent *);
	void changeToNthLayer(int);
	void labMouseClicked(QMouseEvent *);
	void labMouseMoved(QMouseEvent *);
	void updateSubDisplay(QMouseEvent *);
	void writeTxtFile();
	void readPrevFile();
	void readNextFile();


private:
	QSettings* appsettings;
	QString fileName;

	std::vector<cv::Mat> vomat;//original
	std::vector<cv::Mat> vfmat;//filterd
	//std::vector<cluster2d> vc2;
	//std::vector<cluster3d> vc3;

	QPixmap pix1;
	QPixmap pix2;
	QPixmap pix3;

	QClickableLabel* lab_img;
	QLabel* lab_img_o;
	QLabel* lab_img_f;

	QLabel* lab_pix;
	QLabel* lab_pix_cl;
	QLabel* lab_pix_dr;
	QLabel* lab_pix_unit;

	QLabel* lab_stg;
	QLabel* lab_stg_cl;
	QLabel* lab_stg_dr;
	QLabel* lab_stg_unit;

	QLineEdit* line_file;
	QPushButton* but_file;
	QLCDNumber* lcd_img;
	QCheckBox*  chk_enlarge;
	QCheckBox*  chk_filterd;

	QPushButton* but_up;
	QPushButton* but_down;
	QPushButton* but_make;
	QPushButton* but_writxt;

	QTextEdit* txt_clicked;
	QSlider* sli_z;

	QPushButton* but_prevfile;
	QPushButton* but_nextfile;


	int ipict;

	QString document_unit;

	int lx,ly,lz;
	int wi;
	int he;
	int start_x, start_y, start_z;
	int end_x, end_y, end_z;
	double um_px, um_py, um_pz;
	double viewx, viewy, viewz;
	double Sh;

	void Init();
	void getTheDarkestZ(int x, int y, int z, int range);

	int getTheOrdinalInCurrentDir();
	bool readIthFileInCurrentDir(int i);


};

#endif // UENO_VIEWER_H
