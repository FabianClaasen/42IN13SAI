#include "TokenizerController.h"

TokenizerController::TokenizerController(std::string filename)
{
	std::list<TokenDefinition> definitions = Grammar::getGrammar();
	std::vector<TokenPartner> partners = Grammar::getPartners();

	tokenizer = new Tokenizer(filename, definitions, partners);
}

void TokenizerController::Tokenize()
{
	tokenizer->Tokenize();
}

std::vector<std::shared_ptr<Token>> TokenizerController::GetCompilerTokens()
{
	return tokenizer->GetTokenList();
}

bool TokenizerController::HasExceptions()
{
    return tokenizer->HasExceptions();
}

TokenizerController::~TokenizerController()
{
	delete tokenizer;
	tokenizer = nullptr;
}
