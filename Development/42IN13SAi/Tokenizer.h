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
	int line_number;
	int line_position;
	int level;
	std::string line_remaining;

    std::ifstream file;
    std::list<TokenDefinition> token_definitions;
    std::list<TokenPartner> token_partners;
    
    std::list<Token> *token_list;
    
    void nextLine();
    Token* findPartner(TokenType &token_type, int level);
	bool shouldFindPartner(TokenType type);
    std::string ltrim(std::string &s);
    std::string rtrim(std::string &s);
    std::string trim(std::string &s);
    
public:
    Tokenizer(std::string fileLocation, std::list<TokenDefinition> definitions, std::list<TokenPartner> partners);
	virtual ~Tokenizer();

    void Tokenize();
    std::list<Token>* GetTokenList();
};