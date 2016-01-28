#include "nagara_viewer.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	nagara_viewer w;
	w.show();
	return a.exec();
}
