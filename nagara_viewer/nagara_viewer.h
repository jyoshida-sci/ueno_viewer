#ifndef NAGARA_VIEWER_H
#define NAGARA_VIEWER_H

#include <QtWidgets/QWidget>
#include "ui_nagara_viewer.h"

#include "..\opencv2411.h"

class QSettings;
class QImage;
class QLabel;
class QPixmap;
class QLineEdit;
class QPushButton;
class QLCDNumber;
class QCheckBox;



class nagara_viewer : public QWidget
{
	Q_OBJECT

public:
	nagara_viewer(QWidget *parent = 0);
	~nagara_viewer();

signals:
	void wheelEvent(QWheelEvent *);


private slots:
	bool updateImg(int v, int i);//v:view, i:depth
	bool ImgUp();
	bool ImgDown();
	bool loadImg();
	bool changeLayer(QWheelEvent *);


private:

	QSettings* appsettings;
	QString dirName;
	QString fileName;

//	Ui::nagara_viewerClass ui;
	std::vector<char*> vbd;//vector binarized data

	QImage img1;
	QImage img2;
	QImage img3;

	QPixmap pix1;
	QPixmap pix2;
	QPixmap pix3;

	QLabel* lab_img1;
	QLabel* lab_img2;
	QLabel* lab_img3;

	QLineEdit* line_file;
	QPushButton* but_file;
	QLCDNumber* lcd_ipict;
	QCheckBox* chk_halfsize;


	QPushButton* but_up;
	QPushButton* but_down;


	int ipict;
	int nlayer;
	int wi = 2048;
	int he = 1080;
	int ppp = wi*he;//pixel per page
	int bpp = ppp / 8;//byte per page

	unsigned char* chard;//vector 8bit-char data

	bool Init();
};

#endif // NAGARA_VIEWER_H
