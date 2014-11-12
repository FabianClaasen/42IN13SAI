#include <QtWidgets>
#include <QMenu>

#include "MainWindow.h"
#include "CodeEditor.h"
#include "TokenizerController.h"
#include "Compiler.h"
#include "VirtualMachine.h"

MainWindow::MainWindow(QWidget *parent)
{
	showMenuBar();

	codeEditor = new CodeEditor();

	this->setCentralWidget(codeEditor);
}

void MainWindow::showMenuBar()
{
	// Create menu bar
	QMenuBar* menu = new QMenuBar();

	// Create action and connect
	runAction = menu->addAction("Run");
	clearAction = menu->addAction("Clear console");

	// Set the menu bar on ui from left to right
	this->setMenuBar(menu);
	menu->setLayoutDirection(Qt::LeftToRight);
}

QAction *MainWindow::getRunAction()
{
	return runAction;
}

QAction *MainWindow::getClearAction()
{
	return clearAction;
}

QString MainWindow::getText()
{
	return codeEditor->toPlainText();
}

MainWindow::~MainWindow()
{
	// Empty
}