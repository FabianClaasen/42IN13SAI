//
//  main.cpp
//  tokenizer
//
#include <iostream>
#include "TokenizerController.h"
#include "Compiler.h"
#include "VirtualMachine.h"

int main(int argc, const char * argv[])
{
	//std::string fileName("C:\\Users\\Sjoerd\\Dropbox\\42IN13SAI\\Testen\\Compiler\\while_loop.txt");
	std::string fileName("C:\\Users\\stefan\\Dropbox\\42IN13SAI\\Testen\\Tokenizer\\test.txt");
	//std::string fileName("C:\\Users\\Fabian Claasen\\Dropbox\\42IN13SAI\\Testen\\Tokenizer\\test.txt");
	//std::string fileName("C:\\Users\\stefan\\Dropbox\\42IN13SAI\\Testen\\Tokenizer\\test.txt");
	std::string fileName("C:\\Users\\Fabian Claasen\\Dropbox\\42IN13SAI\\Testen\\test.txt");
    
#ifndef _WIN32
    fileName = "/Users/Alex/Documents/test.txt";
#endif
    
	TokenizerController *tokenizer_controller = new TokenizerController(fileName);
	tokenizer_controller->Tokenize();

	Compiler compiler = Compiler(tokenizer_controller->GetCompilerTokens());
	compiler.Compile();

	VirtualMachine virtual_machine = 
		VirtualMachine(compiler.GetSymbolTable(), compiler.GetSubroutineTable(), compiler.GetCompilerNodes());
	//virtualMachineController

    // Show tokenized items
	std::vector<Token> tokens = tokenizer_controller->GetCompilerTokens();
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
    }

    return 0;
}
