//
//  main.cpp
//  tokenizer
//

#include <QApplication> //qt
#include "MainWindow.h" //qt

#include <stdlib.h>

#ifdef _WIN32
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#include <iostream>
#include "TokenizerController.h"
#include "Compiler.h"
#include "VirtualMachine.h"
#include <QShortcut> //qt
#include <QtWidgets>


int main(int argc, const char * argv[])
{
	QApplication app(argc, 0);

	MainWindow mainWindow;

	mainWindow.setWindowTitle(QObject::tr("Short C Editor"));

	mainWindow.resize(640, 360);

	mainWindow.show();

	app.exec();
}
