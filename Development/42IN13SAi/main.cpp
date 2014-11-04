//
//  main.cpp
//  tokenizer
//

#include <QtWidgets\QApplication> //qt
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

	mainWindow.setWindowTitle(QObject::tr("Compiler | Press F5 to run"));

	mainWindow.resize(640, 360);

	mainWindow.show();

	app.exec();

#ifdef _WIN32
	// Memory leaks notifier
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	// Load the file
	//std::string fileName("C:\\Users\\Sjoerd\\Dropbox\\42IN13SAI\\Testen\\Compiler\\decleration.txt");
	//std::string fileName("C:\\Users\\stefan\\Dropbox\\42IN13SAI\\Testen\\Tokenizer\\test.txt");
	//std::string fileName("C:\\Users\\Fabian Claasen\\Dropbox\\42IN13SAI\\Testen\\Tokenizer\\testvar.txt");
	//std::string fileName("C:\\Users\\stefan\\Dropbox\\42IN13SAI\\Testen\\Tokenizer\\testvar.txt");
	std::string fileName("C:\\Users\\Fabian Claasen\\Dropbox\\42IN13SAI\\Testen\\while_loop.txt");
    
#ifndef _WIN32
    fileName = "/Users/Alex/Documents/test.txt";
#endif
    
	

	//QString gen_code = mainWindow.getText();
	//QFile file;
	//file.setFileName("number1.txt");
	//if (file.exists())
	//{
	//	file.remove();
	//}
	//file.open(QIODevice::ReadWrite | QIODevice::Text);
	//QTextStream stream(&file);
	//stream << gen_code;
	//file.close();
	//QFileInfo info(file);
	//// TODO wait for executing because code is running immediately
	//std::string filepath = info.absoluteFilePath().toStdString();
	//std::string input(filepath);

	// Tokenize the code
	TokenizerController *tokenizer_controller = new TokenizerController(fileName);
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

    return 0;
}
