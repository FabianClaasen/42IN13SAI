//
//  main.cpp
//  tokenizer
//
#include <iostream>
#include "TokenizerController.h"
#include "Compiler.h"

int main(int argc, const char * argv[])
{
    std::string fileName("C:\\Users\\Sjoerd\\Dropbox\\42IN13SAI\\Testen\\test.txt");
    
#ifndef _WIN32
    fileName = "/Users/Alex/Documents/test.txt";
#endif
    
	TokenizerController *tokenizer_controler = new TokenizerController(fileName);
	tokenizer_controler->Tokenize();

	Compiler compiler = Compiler(tokenizer_controler->GetCompilerTokens());
	compiler.Compile();

    // Show tokenized items
	std::vector<Token> tokens = tokenizer_controler->GetCompilerTokens();
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
