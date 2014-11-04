//
//  main.cpp
//  tokenizer
//

#include <QtWidgets\QApplication> //qt
#include "MainWindow.h" //qt
#include <QShortcut> //qt
#include <QtWidgets>

#ifdef _WIN32
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif


int main(int argc, const char * argv[])
{

#ifdef _WIN32
	// Memory leaks notifier
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	QApplication app(argc, 0);

	MainWindow mainWindow;

	mainWindow.setWindowTitle(QObject::tr("Compiler | Press F5 to run"));

	mainWindow.resize(640, 360);

	mainWindow.show();

	return app.exec();
}
