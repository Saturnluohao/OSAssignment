#include "elevatorUI.h"
#include <QtWidgets/QApplication>


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	guider g;
	g.setWindowTitle("Navigator");
	g.resize(250, 300);
	g.show();
	return a.exec();
}
