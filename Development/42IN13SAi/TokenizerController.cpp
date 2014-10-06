#include "TokenizerController.h"


TokenizerController::TokenizerController(std::string filename)
{
	std::list<TokenDefinition> definitions = Grammar::getGrammar();
	std::list<TokenPartner> partners = Grammar::getPartners();

	tokenizer = new Tokenizer(filename, definitions, partners);
}

void TokenizerController::Tokenize()
{
	tokenizer->Tokenize();
}

std::list<Token> TokenizerController::GetCompilerTokens()
{
	return *tokenizer->GetTokenList();
}

TokenizerController::~TokenizerController()
{
	delete tokenizer;
	tokenizer = nullptr;
}
