//
//  main.cpp
//  tokenizer
//
//  Created by Alex Nagelkerke on 19-09-14.
//  Copyright (c) 2014 Alex Nagelkerke. All rights reserved.
//

#include <iostream>
#include "TokenizerController.h"

int main(int argc, const char * argv[])
{
	TokenizerController *tokenizer_controler = new TokenizerController("C:\\Users\\Alex\\Documents\\test.txt");
	tokenizer_controler->Tokenize();
    
    // Show tokenized items
	std::list<Token> tokens = tokenizer_controler->GetCompilerTokens();
    std::list<Token>::iterator tokenIt;
    for (tokenIt = tokens.begin(); tokenIt != tokens.end(); ++tokenIt)
    {
        Token token = *tokenIt;
        Token partner;
        if (token.Partner)
            partner = *token.Partner;
        
        std::cout << "line: " + std::to_string(token.LineNumber) +  " line position: " + std::to_string(token.LinePosition) + " level: " + std::to_string(token.Level) + " type: " + std::to_string(token.Type) + " Value: " + token.Value + " Partner: " + partner.Value << std::endl;
    }
    
    return 0;
}
