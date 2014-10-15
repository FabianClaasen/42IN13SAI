#pragma once
#include <vector>

#include "Token.h"
#include "Tokenizer.h"
#include "Grammar.h"

class TokenizerController
{
private:
	Tokenizer *tokenizer;
public:
	TokenizerController(std::string filename);
	virtual ~TokenizerController();

	void Tokenize();
	std::vector<Token> GetCompilerTokens();
};

