#pragma once
#include <list>

#include "Token.h"
#include "Tokenizer.h"
#include "Grammar.h"

class TokenizerController
{
private:
	Tokenizer *tokenizer;

    std::vector<std::string> exceptions;
public:
	TokenizerController(std::string filename);
	virtual ~TokenizerController();

	void Tokenize();
	std::vector<Token> GetCompilerTokens();

    std::vector<std::string> getExceptions() { return exceptions; }
};

