#include "os3.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	OS3 w;
	w.show();
	return a.exec();
}
