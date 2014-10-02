#include "Compiler.h"
#include "CompilerNode.h"
#include "Token.h"

#include <string>
#include <list>

using namespace std;

std::list<Token> *tokens;

Token lookAhead;

void compile()
{
	string expression = "";
	string identifier = "";
	string value = "";

	std::list<Token>::iterator tokenIt;
	for (tokenIt = tokens->begin(); tokenIt != tokens->end(); ++tokenIt)
	{
		Token token = *tokenIt;
		Token partner;

		if (token.Partner)
			partner = *token.Partner;

		if (token.Type == TokenType::Identifier)
		{
			identifier = token.Value;
		}

		if (token.Type == TokenType::Equals)
		{
			expression = "$Equals";
		}

		if (token.Type == TokenType::Integer || token.Type == TokenType::Double)
		{
			value = token.Value;
		}

		CompilerNode* node = new CompilerNode(expression, identifier, value);

		expression = "";
		identifier = "";
		value = "";
	}
}

Compiler::Compiler()
{
	compile();
}


Compiler::~Compiler()
{
}
