//
//  Tokenizer.h
//  tokenizer
//
#pragma once

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
	int lineNumber;
	int linePosition;
	int level;
	std::string lineRemaining;

    std::ifstream file;
    std::list<TokenDefinition> tokenDefinitions;
    std::list<TokenPartner> tokenPartners;
    
    std::list<Token> *tokenList;
    
    void NextLine();
    Token* FindPartner(TokenType &tokenType, int level);
	bool ShouldFindPartner(TokenType type);
    std::string ltrim(std::string &s);
    std::string rtrim(std::string &s);
    std::string trim(std::string &s);
    
public:
    Tokenizer(std::string fileLocation, std::list<TokenDefinition> definitions, std::list<TokenPartner> partners);
	virtual ~Tokenizer();

    void Tokenize();
    std::list<Token>* GetTokenList();
};