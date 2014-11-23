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

// The tokenize function will fill the tokenList with all the tokens
// gathered from the code send to it.
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

				// Check if the level should be raised
				if (definition.myTokenType == MyTokenType::OpenBracket || definition.myTokenType == MyTokenType::OpenCurlyBracket || definition.myTokenType == MyTokenType::OpenMethod)
					level++;

				// Find a partner
				Token* partner = nullptr;
				if (ShouldFindPartner(definition.myTokenType))
				{
					try {
						Token* temp = FindPartner(definition.myTokenType, level);
						if (temp->Partner != nullptr)
							partner = temp;
					}
					catch (const PartnerNotFoundException &e) {
						// Catch the exception and rethrow
						throw;
					}
				}

				// Create token
				std::string token_value = lineRemaining.substr(0, matched);
				tokenVector.push_back(Token(lineNumber, linePosition, level, token_value, definition.myTokenType, partner));
                
                // if partner found, give this token to partner
                if (partner)
                    partner->Partner = &tokenVector.back();

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
		if (!match)
		{
			throw ParseException("Unrecognized character '" + lineRemaining.substr(0) + "' on line " + std::to_string(lineNumber) + " at position " + std::to_string(linePosition));
			lineRemaining = "";
		}
	}

	try {
		CheckClosingPartners();
	}
	catch (const PartnerNotFoundException &e) {
		// Catch the exception and rethrow
		throw;
	}
}


// Check for closing partners missing
void Tokenizer::CheckClosingPartners()
{	
	for (Token t : tokenVector)
	{
		if (ShouldFindPartnerR(t.Type))
		{
			try {
				TryFindPartner(t.Type, t.Level);
			}
			catch (const PartnerNotFoundException &e) {
				// Catch the exception and rethrow
				throw;
			}
		}
	}
}

// Find a partner for the current token.
// @param
//  type: this is the type of the token where you need to find a match for.
//	level: this is the level of the myTokenType, the partner needs to be on the same level.
Token* Tokenizer::FindPartner(MyTokenType &type, int level)
{
    std::list<TokenPartner>::const_iterator token_partner;
    for (TokenPartner tokenPartner : tokenPartners)
    {
		if (tokenPartner.token == type)
        {
            std::vector<Token>::reverse_iterator tokenIt;
            for (tokenIt = tokenVector.rbegin(); tokenIt != tokenVector.rend(); ++tokenIt)
            {
                if (tokenIt->Type == tokenPartner.partner && tokenIt->Level == level)
                    return &(*tokenIt);
            }
        }
    }
    char buffer[1024];
    
#ifdef _WIN32
	_snprintf(buffer, sizeof(buffer), "Partner not found for %s on line %s at position %s", TokenToString(type).c_str(), std::to_string(lineNumber).c_str(), std::to_string(linePosition).c_str());
#else
	snprintf(buffer, sizeof(buffer), "Partner not found for %s on line %s at position %s", TokenToString(type).c_str(), std::to_string(lineNumber).c_str(), std::to_string(linePosition).c_str());
#endif
    
	throw PartnerNotFoundException(buffer);
}

// Find a partner for the current token.
// @param
//  type: this is the type of the token where you need to find a match for.
//	level: this is the level of the myTokenType, the partner needs to be on the same level.
void Tokenizer::TryFindPartner(MyTokenType &type, int level)
{
	std::list<TokenPartner>::const_iterator token_partner;
	for (TokenPartner tokenPartner : tokenPartners)
	{
		if (tokenPartner.partner == type)
		{
			std::vector<Token>::iterator tokenIt;
			for (tokenIt = tokenVector.begin(); tokenIt != tokenVector.end(); ++tokenIt)
			{
				if (tokenIt->Type == tokenPartner.token && tokenIt->Level == level)
				{	// FOUND A PARTNER
					return;
				}
			}
		}
	}

	// Didn't find a partner:
	char buffer[1024];

#ifdef _WIN32
	_snprintf(buffer, sizeof(buffer), "Partner not found for %s on line %s at position %s", TokenToString(type).c_str(), std::to_string(lineNumber).c_str(), std::to_string(linePosition).c_str());
#else
	snprintf(buffer, sizeof(buffer), "Partner not found for %s on line %s at position %s", TokenToString(type).c_str(), std::to_string(lineNumber).c_str(), std::to_string(linePosition).c_str());
#endif

	throw PartnerNotFoundException(buffer);
}

// Check if the current token is able to have a partner
// @Param
//	type: the function will check if this type can have a partner.
bool Tokenizer::ShouldFindPartner(MyTokenType &type)
{
    std::vector<MyTokenType> types{ MyTokenType::CloseCurlyBracket, MyTokenType::CloseMethod, MyTokenType::CloseBracket, MyTokenType::Else, MyTokenType::ElseIf };
	return std::find(types.begin(), types.end(), type) != types.end();
}

// Check if the current token is able to have a partner
// @Param
//	type: the function will check if this type can have a partner.
bool Tokenizer::ShouldFindPartnerR(MyTokenType &type)
{
	std::vector<MyTokenType> types{ MyTokenType::OpenCurlyBracket, MyTokenType::OpenMethod, MyTokenType::OpenBracket };
	return std::find(types.begin(), types.end(), type) != types.end();
}

// This function will read a new line from the file.
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

std::vector<Token> Tokenizer::GetTokenList()
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

Tokenizer::~Tokenizer()
{
	file.close();
}