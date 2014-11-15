#include <QtWidgets>
#include <QMenu>

#include "MainWindow.h"
#include "CodeEditor.h"
#include "TokenizerController.h"
#include "Compiler.h"
#include "VirtualMachine.h"

MainWindow::MainWindow(QWidget *parent)
{
	ShowMenuBar();
	codeEditor = new CodeEditor();
	highlighter = new Highlighter(codeEditor->document());

	this->setCentralWidget(codeEditor);
}

void MainWindow::ShowMenuBar()
{
	// Create menu bar
	QMenuBar* menu = nullptr;

	#ifdef _WIN32
		menu = new QMenuBar();
	#else
        // Mac OS X needs menubar without a parent
		menu = new QMenuBar(0);
    
        // Also needs a menu to show the items, doesn't work with only actions
		QMenu* mainMenu = menu->addMenu("File");
	#endif

	// Create action and connect
	fileMenu = menu->addMenu("File");
	openFile = fileMenu->addAction("Open file");
	runAction = menu->addAction("Run");
	clearAction = menu->addAction("Clear console");

	#ifndef _WIN32
        // Add the actions to the menu
		mainMenu->addAction(runAction);
		mainMenu->addAction(clearAction);
	#endif

	// Set the menu bar on ui from left to right
	this->setMenuBar(menu);
	menu->setLayoutDirection(Qt::LeftToRight);
}

QAction* MainWindow::GetRunAction()
{
	return runAction;
}

QAction* MainWindow::GetClearAction()
{
	return clearAction;
}

QAction* MainWindow::GetLoadFileAction()
{
	return openFile;
}

QString MainWindow::GetText()
{
	return codeEditor->toPlainText();
}

QString MainWindow::OpenFileDialog()
{
	return QFileDialog::getOpenFileName(this, tr("Open file"), "", tr("Text Files (*.txt)"));
}

void MainWindow::SetText(QString text)
{
	codeEditor->clear();
	codeEditor->insertPlainText(text);
}

MainWindow::~MainWindow()
{
	// Empty
}