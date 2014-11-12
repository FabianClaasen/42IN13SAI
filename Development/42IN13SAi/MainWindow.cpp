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

	// Get the file from the stream and convert to std::string
	std::string input(getFileFromStream().toStdString());

	TokenizerController *tokenizer_controller = new TokenizerController(input);
	try {
		// Tokenize the code		
		tokenizer_controller->Tokenize();
	}
	catch (const std::exception& e)
	{
		puts(e.what());
		delete(tokenizer_controller);	
		return;
	}

	// Run the compiler
	Compiler compiler = Compiler(tokenizer_controller->GetCompilerTokens());
	compiler.Compile();

	// Create action and connect
	runAction = menu->addAction("Run");
	clearAction = menu->addAction("Clear console");

	// Set the menu bar on ui from left to right
	this->setMenuBar(menu);
	menu->setLayoutDirection(Qt::LeftToRight);
	// Run the virtual machine with the compilernodes
	VirtualMachine virtual_machine = VirtualMachine(compiler.GetSymbolTable(), compiler.GetSubroutineTable(), compiler.GetCompilerNodes());
	virtual_machine.ExecuteCode();
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
	// Create menu bar
    QMenuBar* menu = nullptr;
#ifdef _WIN32
	menu = new QMenuBar();
#else
    menu = new QMenuBar(0);
    
    QMenu* mainMenu = menu->addMenu("File");
#endif

	// Create action and connect
	QAction *runAction = menu->addAction("Run");
	QAction *clearAction = menu->addAction("Clear console");

	connect(runAction, SIGNAL(triggered()), this, SLOT(execute()));
	connect(clearAction, SIGNAL(triggered()), this, SLOT(clearConsole()));
    
#ifndef _WIN32
    mainMenu->addAction(runAction);
    mainMenu->addAction(clearAction);
#endif

	// Set the menu bar on ui from left to right
	this->setMenuBar(menu);
	menu->setLayoutDirection(Qt::LeftToRight);
}

MainWindow::~MainWindow()
{
	// Empty
}