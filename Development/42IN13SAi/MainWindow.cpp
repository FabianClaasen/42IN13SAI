#include <QtWidgets>
#include <QShortcut>
#include <qstring.h>
#include <QMessageBox>

#include "MainWindow.h"
#include "CodeEditor.h"




#include <iostream>
#include "TokenizerController.h"
#include "Compiler.h"
#include "VirtualMachine.h"

MainWindow::MainWindow(QWidget *parent)
{
	QShortcut *shortcut = new QShortcut(QKeySequence(Qt::Key_F5), this);
	codeEditor = new CodeEditor();
	connect(shortcut, SIGNAL(activated()), this, SLOT(execute()));
	
	this->setCentralWidget(codeEditor);
}

void MainWindow::execute()
{
	// Excute typed code
	#ifndef _WIN32
	fileName = "/Users/Alex/Documents/test.txt";
	#endif

	// Get the file from the stream and convert to std::string
	std::string input(getFileFromStream().toStdString());

	// Tokenize the code
	TokenizerController *tokenizer_controller = new TokenizerController(input);
	tokenizer_controller->Tokenize();

	// Run the compiler
	Compiler compiler = Compiler(tokenizer_controller->GetCompilerTokens());
	compiler.Compile();

	// Delete the tokenizer controller
	//delete(tokenizer_controller);

	// Run the virtual machine with the compilernodes
	VirtualMachine virtual_machine =
		VirtualMachine(compiler.GetSymbolTable(), compiler.GetSubroutineTable(), compiler.GetCompilerNodes());
	virtual_machine.ExecuteCode();
}

QString MainWindow::getText()
{
	return codeEditor->toPlainText();
}

QString MainWindow::getFileFromStream()
{
	QString gen_code = getText();
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

	//Return the filePath
	return filepath;
}

MainWindow::~MainWindow()
{
	
}