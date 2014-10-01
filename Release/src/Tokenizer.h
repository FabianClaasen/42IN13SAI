//
//  Tokenizer.h
//  tokenizer
//
//  Created by Alex Nagelkerke on 19-09-14.
//  Copyright (c) 2014 Alex Nagelkerke. All rights reserved.
//

#include <fstream>
#include <list>
#include <string>
#include "TokenDefinition.h"
#include "Token.h"
#include "TokenPartner.h"
#include "PartnerNotFoundException.h"
#include "ParseException.h"

class Tokenizer
{
private:
    std::ifstream file;
    std::list<TokenDefinition> tokenDefinitions;
    std::list<TokenPartner> tokenPartners;
    
    std::list<Token> *tokenList;
    
    int lineNumber;
    int linePosition;
    int level;
    std::string lineRemaining;
    
    void nextLine();
    Token* findPartner(TokenType &tokenType, int level);
    std::string ltrim(std::string &s);
    std::string rtrim(std::string &s);
    std::string trim(std::string &s);
    
public:
    Tokenizer(std::string fileLocation, std::list<TokenDefinition> definitions, std::list<TokenPartner> partners);
    void Tokenize();
    std::list<Token>* GetTokenList();
};