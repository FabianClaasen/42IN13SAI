#include <iostream>
#include <QtWidgets>
#include <QShortcut>

#include "MainController.h"
#include "TokenizerController.h"
#include "Compiler.h"
#include "VirtualMachine.h"

MainController::MainController() : QObject()
{
	mainWindow.setWindowTitle(QObject::tr("Short C Editor"));
	mainWindow.resize(640, 360);
	mainWindow.show();

	setup();
}

void MainController::setup()
{
	QShortcut *shortcut = new QShortcut(QKeySequence(Qt::Key_F5), &mainWindow);
	connect(shortcut, SIGNAL(activated()), this, SLOT(execute()));
    
#ifndef _WIN32
    // Mac OS X cmd + R for running program like in xcode
    QShortcut *macShortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_R), &mainWindow);
    connect(macShortcut, SIGNAL(activated()), this, SLOT(execute()));
#endif

	connect(mainWindow.getRunAction(), SIGNAL(triggered()), this, SLOT(execute()));
	connect(mainWindow.getClearAction(), SIGNAL(triggered()), this, SLOT(clearConsole()));
}

void MainController::execute()
{
	// Excute typed code
	// Get the file from the stream and convert to std::string
	std::string input(getFileFromStream().toStdString());

	TokenizerController *tokenizer_controller = new TokenizerController(input);

	try
	{
		tokenizer_controller->Tokenize(); // Tokenize
	}
	catch (const PartnerNotFoundException& e)
	{
		delete(tokenizer_controller);

		return;
	}

	// Run the compiler
	Compiler compiler = Compiler(tokenizer_controller->GetCompilerTokens());
	compiler.Compile();

	// Delete the tokenizer controller
	delete(tokenizer_controller);

	// Run the virtual machine with the compilernodes
	VirtualMachine virtual_machine = VirtualMachine(compiler.GetSymbolTable(), compiler.GetSubroutineTable(), compiler.GetCompilerNodes());
	virtual_machine.ExecuteCode();
}

void MainController::clearConsole()
{
	system("cls");
}

QString MainController::getFileFromStream()
{
	QString gen_code = mainWindow.getText();

	QFile file;

	file.setFileName("number1.txt");

	if (file.exists())
	{
		file.remove();
	}

	file.open(QIODevice::ReadWrite | QIODevice::Text);
	QTextStream stream(&file);

	stream << gen_code;
	file.close();

	QFileInfo info(file);

	QString filepath = info.absoluteFilePath();

	return filepath;
}

MainController::~MainController()
{
	// Empty
}