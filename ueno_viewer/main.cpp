#include "ueno_viewer.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	ueno_viewer w;
	w.show();
	return a.exec();
}
