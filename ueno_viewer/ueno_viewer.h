#ifndef UENO_VIEWER_H
#define UENO_VIEWER_H

#include <QtWidgets/QWidget>
#include "ui_ueno_viewer.h"

class ueno_viewer : public QWidget
{
	Q_OBJECT

public:
	ueno_viewer(QWidget *parent = 0);
	~ueno_viewer();

private:
	Ui::ueno_viewerClass ui;
};

#endif // UENO_VIEWER_H
