#include "Tokenizer.h"

Tokenizer::Tokenizer(std::string fileLocation, std::list<TokenDefinition> definitions, std::vector<TokenPartner> partners)
{
    // set defaults
    lineNumber      = 0;
    linePosition    = 1;
    level            = 1;
    lineRemaining   = "";
    
    tokenDefinitions = definitions;
    tokenPartners    = partners;

	file.open(fileLocation);
    
    NextLine();
}

void Tokenizer::Tokenize()
{
	while (lineRemaining.length() != 0)
	{
		bool match = false;
		boost::algorithm::trim(lineRemaining);
		if (lineRemaining.length() == 0)
		{
			NextLine();
			continue;
		}

		// Loop through all the token definitions
		std::list<TokenDefinition>::const_iterator definitionIterator;
		for (definitionIterator = tokenDefinitions.begin(); definitionIterator != tokenDefinitions.end(); ++definitionIterator)
		{
			TokenDefinition definition = *definitionIterator;
			int matched = definition.matcher.Match(lineRemaining);

			if (matched > 0)
			{
                match = true;
                if (definition.myTokenType == MyTokenType::Extended_ASCII)
                {
                    Diag(ExceptionEnum::err_unkown_char) << lineRemaining.substr(0, 1) << lineNumber << linePosition;
                    
                    lineRemaining = "";
                    NextLine();
                    
                    break;
                }

				if (definition.myTokenType == MyTokenType::Comment)
				{
					lineRemaining = "";
					NextLine();
					break;
				}
				
				// Check if the level should be raised
				if (definition.myTokenType == MyTokenType::OpenBracket || definition.myTokenType == MyTokenType::OpenCurlyBracket || definition.myTokenType == MyTokenType::OpenMethod)
					level++;

				// Find a partner
				std::shared_ptr<Token> partner = nullptr;
				if (ShouldFindPartner(definition.myTokenType))
				{
                    std::shared_ptr<Token> temp = FindPartner(definition.myTokenType, level);
                    if (temp)
                    {
                        if (temp->Partner.lock() == nullptr)
                            partner = temp;
                    }
				}

				// Create token
				std::string token_value = lineRemaining.substr(0, matched);
                tokenVector.push_back(std::make_shared<Token>(lineNumber, linePosition, level, token_value, definition.myTokenType, partner));
                
                // if partner found, give this token to partner
                if (partner)
                    partner->Partner = std::shared_ptr<Token>(tokenVector.back());

				// Check if the level should be lowered
				if (definition.myTokenType == MyTokenType::CloseBracket || definition.myTokenType == MyTokenType::CloseCurlyBracket || definition.myTokenType == MyTokenType::CloseMethod)
					level--;

				// Change your position and line
				linePosition += matched;
				lineRemaining = lineRemaining.substr(matched);
				if (lineRemaining.length() == 0)
					NextLine();
				
				break;
			}
		}
        // Throw an exception if the target couldnt be parsed as a token.
        // Continue to next line, shouldn't tokenize anything after unkown character
        if (!match)
        {
            Diag(ExceptionEnum::err_unkown_char) << lineRemaining.substr(0, 1) << lineNumber << linePosition;
            NextLine();
        }
	}

	// Check for partners
    CheckClosingPartners();

	// Close the file
	file.close();
}

void Tokenizer::CheckClosingPartners()
{	
	for (std::shared_ptr<Token> t : tokenVector)
	{
		if (ShouldFindPartnerR(t->Type))
		{
            TryFindPartner(t);
		}
	}
}

std::shared_ptr<Token> Tokenizer::FindPartner(MyTokenType &type, int level)
{
    std::list<TokenPartner>::const_iterator token_partner;
    for (TokenPartner tokenPartner : tokenPartners)
    {
		if (tokenPartner.token == type)
        {
            std::vector<std::shared_ptr<Token>>::reverse_iterator tokenIt;
            for (tokenIt = tokenVector.rbegin(); tokenIt != tokenVector.rend(); ++tokenIt)
            {
                if ((*tokenIt)->Type == tokenPartner.partner && (*tokenIt)->Level == level)
                    return (*tokenIt);
            }
        }
    }

    Diag(ExceptionEnum::err_partner_not_found) << TokenToString(type) << lineNumber << linePosition;
    return std::shared_ptr<Token>();
}

void Tokenizer::TryFindPartner(std::shared_ptr<Token> token)
{
	std::list<TokenPartner>::const_iterator token_partner;
	for (TokenPartner tokenPartner : tokenPartners)
	{
		if (tokenPartner.partner == token->Type)
		{
			std::vector<std::shared_ptr<Token>>::iterator tokenIt;
			for (tokenIt = tokenVector.begin(); tokenIt != tokenVector.end(); ++tokenIt)
			{
                if ((*tokenIt)->Type == tokenPartner.token && (*tokenIt)->Level == token->Level)
                    // Partner found
                    return ;
			}
		}
	}

	// Didn't find a partner:
    Diag(ExceptionEnum::err_partner_not_found) << token->Value << token->LineNumber << token->LinePosition;
}

bool Tokenizer::ShouldFindPartner(MyTokenType &type)
{
    std::vector<MyTokenType> types{ MyTokenType::CloseCurlyBracket, MyTokenType::CloseMethod, MyTokenType::CloseBracket, MyTokenType::Else, MyTokenType::ElseIf };
	return std::find(types.begin(), types.end(), type) != types.end();
}

bool Tokenizer::ShouldFindPartnerR(MyTokenType &type)
{
	std::vector<MyTokenType> types{ MyTokenType::OpenCurlyBracket, MyTokenType::OpenMethod, MyTokenType::OpenBracket };
	return std::find(types.begin(), types.end(), type) != types.end();
}

void Tokenizer::NextLine()
{
	while (std::getline(file, lineRemaining))
	{
		++lineNumber;
		linePosition = 1;

		if (lineRemaining.length() > 0)
			break;
	}
}

std::vector<std::shared_ptr<Token>> Tokenizer::GetTokenList()
{
    return tokenVector;
}

std::string Tokenizer::TokenToString(MyTokenType type)
{
	switch (type)
	{		
	case MyTokenType::OpenBracket:
		return "(";
	case MyTokenType::CloseBracket:
		return ")";
	case MyTokenType::OpenCurlyBracket:
		return "{";
	case MyTokenType::CloseCurlyBracket:
		return "}";
	case MyTokenType::OpenMethod:
		return "[";
	case MyTokenType::CloseMethod:
		return "]";
	case MyTokenType::EOL:
		return ";";
	default:
		return "undefined";
	}
}

bool Tokenizer::HasExceptions()
{
    return hasExceptions;
}

DiagnosticBuilder Tokenizer::Diag(ExceptionEnum exception)
{
    hasExceptions = true;
    return DiagnosticBuilder(exception);
}

Tokenizer::~Tokenizer()
{
	file.close();
}