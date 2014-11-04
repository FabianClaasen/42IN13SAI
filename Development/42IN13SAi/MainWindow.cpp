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
	

	// Load the file
	//std::string fileName("C:\\Users\\Sjoerd\\Dropbox\\42IN13SAI\\Testen\\Compiler\\decleration.txt");
	//std::string fileName("C:\\Users\\stefan\\Dropbox\\42IN13SAI\\Testen\\Tokenizer\\test.txt");
	//std::string fileName("C:\\Users\\Fabian Claasen\\Dropbox\\42IN13SAI\\Testen\\Tokenizer\\testvar.txt");
	//std::string fileName("C:\\Users\\stefan\\Dropbox\\42IN13SAI\\Testen\\Tokenizer\\testvar.txt");
	//std::string fileName("C:\\Users\\Fabian Claasen\\Dropbox\\42IN13SAI\\Testen\\test.txt");

	#ifndef _WIN32
	fileName = "/Users/Alex/Documents/test.txt";
	#endif

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
	// TODO wait for executing because code is running immediately
	std::string filepath = info.absoluteFilePath().toStdString();
	std::string input(filepath);

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

	// Show tokenized items
	/*std::vector<Token> tokens = tokenizer_controller->GetCompilerTokens();
	std::vector<Token>::iterator tokenIt;
	for (tokenIt = tokens.begin(); tokenIt != tokens.end(); ++tokenIt)
	{
	Token token = *tokenIt;
	Token partner;
	if (token.Partner)
	partner = *token.Partner;

	char buffer[1024];

	#ifdef _WIN32
	_snprintf(buffer, sizeof(buffer), "line: %d lineposition: %d level: %d type: %d value: %s Partner: %s", token.LineNumber, token.LinePosition, token.Level, int(token.Type), token.Value.c_str(), partner.Value.c_str());
	#else
	snprintf(buffer, sizeof(buffer), "line: %d lineposition: %d level: %d type: %d value: %s Partner: %s", token.LineNumber, token.LinePosition, token.Level, int(token.Type), token.Value.c_str(), partner.Value.c_str());
	#endif

	std::cout << buffer << std::endl;
	}*/
}

QString MainWindow::getText()
{
	return codeEditor->toPlainText();
}

MainWindow::~MainWindow()
{
	
}