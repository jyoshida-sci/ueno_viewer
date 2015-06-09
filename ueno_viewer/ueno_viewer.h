#ifndef UENO_VIEWER_H
#define UENO_VIEWER_H

#include <QtWidgets/QWidget>
#include "ui_ueno_viewer.h"

#include "..\opencv.h"


class QSettings;
class QLabel;
class QPixmap;
class QLineEdit;
class QPushButton;
class QLCDNumber;
class QCheckBox;
class QTextEdit;
class QClickableLabel;


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
	bool ImgUp();
	bool ImgDown();
	bool loadImg();
	bool changeLayer(QWheelEvent *);
	void displayMousePos(QMouseEvent *);
	void labMouseClicked(QMouseEvent *);
	void labMouseMoved(QMouseEvent *);
	void updateSubDisplay(QMouseEvent *);
	void showContextMenu(QMouseEvent *);

private:
	QSettings* appsettings;

	std::vector<cv::Mat> vomat;//original
	std::vector<cv::Mat> vfmat;//filterd
	//std::vector<cluster2d> vc2;
	//std::vector<cluster3d> vc3;

	QPixmap pix1;
	QPixmap pix2;
	QPixmap pix3;
	QPixmap pix_f;

	QClickableLabel* lab_img;
	QLabel* lab_img_o;
	QLabel* lab_img_f;

	QLabel* lab_pix;
	QLabel* lab_pix_cl;
	QLabel* lab_pix_dr;

	QLineEdit* line_file;
	QPushButton* but_file;
	QLCDNumber* lcd_img;
	QCheckBox*  chk_enlarge;
	QCheckBox*  chk_filterd;

	QPushButton* but_up;
	QPushButton* but_down;
	QPushButton* but_make;

	QTextEdit* txt_clicked;


	int ipict;
	int lx,ly;
	int wi;
	int he;

	void Init();
	void getTheDarkestZ(int x, int y, int z, int range);
};

#endif // UENO_VIEWER_H
