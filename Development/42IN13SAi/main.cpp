//#include <QtWidgets\QApplication>
#ifdef _WIN32
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <stdlib.h>
#endif

#include "MainController.h"
#include <QApplication> //qt
#include "MainWindow.h" //qt

int main(int argc, const char * argv[])
{
	//_CrtSetFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	QApplication app(argc, 0);

	std::unique_ptr<MainController> mainController(new MainController());
	app.exec();
}
