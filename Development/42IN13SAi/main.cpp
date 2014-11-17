//#include <QtWidgets\QApplication>

#include "MainController.h"
#include <QApplication> //qt
#include "MainWindow.h" //qt

#include <stdlib.h>

#ifdef _WIN32
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

int main(int argc, const char * argv[])
{
	QApplication app(argc, 0);

	MainController *mainController = new MainController();

	app.exec();
}
