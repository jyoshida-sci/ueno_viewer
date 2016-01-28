#ifndef NAGARA_VIEWER_H
#define NAGARA_VIEWER_H

#include <QtWidgets/QWidget>
#include "ui_nagara_viewer.h"

class nagara_viewer : public QWidget
{
	Q_OBJECT

public:
	nagara_viewer(QWidget *parent = 0);
	~nagara_viewer();

private:
	Ui::nagara_viewerClass ui;
};

#endif // NAGARA_VIEWER_H
