//
//  main.cpp
//  tokenizer
//
//  Created by Alex Nagelkerke on 19-09-14.
//  Copyright (c) 2014 Alex Nagelkerke. All rights reserved.
//

#include <iostream>
#include "Tokenizer.h"
#include "Grammar.h"

int main(int argc, const char * argv[])
{
    std::list<TokenDefinition>  definitions = Grammar::getGrammar();
    std::list<TokenPartner>     partners    = Grammar::getPartners();
    
    Tokenizer *tokenizer = new Tokenizer("/Users/Alex/test.txt", definitions, partners);
    tokenizer->Tokenize();
    
    // Show tokenized items
    std::list<Token> *tokens = tokenizer->GetTokenList();
    std::list<Token>::iterator tokenIt;
    for (tokenIt = tokens->begin(); tokenIt != tokens->end(); ++tokenIt)
    {
        Token token = *tokenIt;
        Token partner;
        if (token.Partner)
            partner = *token.Partner;
        
        std::cout << "line: " + std::to_string(token.LineNumber) +  " line position: " + std::to_string(token.LinePosition) + " level: " + std::to_string(token.Level) + " type: " + std::to_string(token.Type) + " Value: " + token.Value + " Partner: " + partner.Value << std::endl;
    }
    
    return 0;
}
