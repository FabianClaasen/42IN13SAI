#pragma once

#include <fstream>
#include <list>
#include <vector>
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
    std::vector<TokenPartner> tokenPartners;
    
    std::vector<Token> tokenVector;
    
    void NextLine();
    Token* FindPartner(TokenType &tokenType, int level);
	bool ShouldFindPartner(TokenType type);
    std::string ltrim(std::string &s);
    std::string rtrim(std::string &s);
    std::string trim(std::string &s);
    
public:
    Tokenizer(std::string fileLocation, std::list<TokenDefinition> definitions, std::vector<TokenPartner> partners);
	virtual ~Tokenizer();

    void Tokenize();
    std::vector<Token> GetTokenList();
};