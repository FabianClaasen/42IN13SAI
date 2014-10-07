 //
//  Tokenizer.cpp
//  tokenizer
//
#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <cctype>
#include <functional>
#include <iostream>
#include "Tokenizer.h"

Tokenizer::Tokenizer(std::string fileLocation, std::list<TokenDefinition> definitions, std::list<TokenPartner> partners)
{
    // set defaults
    line_number      = 0;
    line_position    = 1;
    level            = 1;
    line_remaining   = "";
    
    token_definitions = definitions;
    token_partners    = partners;

	file.open(fileLocation);
    
    nextLine();
}

// The tokenize function will fill the token_list with all the tokens
// gathered from the code send to it.
void Tokenizer::Tokenize()
{
	token_list = new std::list<Token>();

	while (line_remaining.length() != 0)
	{
		bool match = false;
		line_remaining = trim(line_remaining);
		if (line_remaining.length() == 0)
		{
			nextLine();
			continue;
		}

		// Loop through all the token definitions
		std::list<TokenDefinition>::const_iterator definitionIterator;
		for (definitionIterator = token_definitions.begin(); definitionIterator != token_definitions.end(); ++definitionIterator)
		{
			TokenDefinition definition = *definitionIterator;
			int matched = definition.matcher->Match(line_remaining);

			if (matched > 0)
			{
				match = true;

				// Check if the level should be raised
				if (definition.tokenType == TokenType::OpenBracket || definition.tokenType == TokenType::OpenCurlyBracket || definition.tokenType == TokenType::OpenMethod)
					level++;

				// Find a partner
				Token *partner = nullptr;
				if (shouldFindPartner(definition.tokenType))
				{
					try {
						partner = findPartner(definition.tokenType, level);
					}
					catch (std::exception &e) {
						std::cout << e.what();
					}
				}

				// Create token
				std::string token_value = line_remaining.substr(0, matched);
				Token *token = new Token(line_number, line_position, level, token_value, definition.tokenType, partner);
				
				// if partner found, give this token to partner
				if (partner)
					partner->Partner = token;
				token_list->push_back(*token);

				// Check if the level should be lowered
				if (definition.tokenType == TokenType::CloseBracket || definition.tokenType == TokenType::CloseCurlyBracket || definition.tokenType == TokenType::CloseMethod)
					level--;

				// Change your position and line
				line_position += matched;
				line_remaining = line_remaining.substr(matched);
				if (line_remaining.length() == 0)
					nextLine();

				break;
			}
		}

		// Throw an exception if the target couldnt be parsed as a token.
		if (!match)
			throw new ParseException("Unrecognized character '" + line_remaining + "' on line " + std::to_string(line_number) + " at position " + std::to_string(line_position));
	}
}

// Find a partner for the current token.
// @param
//  type: this is the type of the token where you need to find a match for.
//	level: this is the level of the tokentype, the partner needs to be on the same level.
Token* Tokenizer::findPartner(TokenType &type, int level)
{
    Token *token = nullptr;
    std::list<TokenPartner>::const_iterator token_partner;
    for (token_partner = token_partners.begin(); token_partner != token_partners.end(); ++token_partner)
    {
        TokenPartner tokenP = *token_partner;
		if (tokenP.token == type)
        {
            std::list<Token>::reverse_iterator tokenIt;
            for (tokenIt = token_list->rbegin(); tokenIt != token_list->rend(); ++tokenIt)
            {
                token = &(*tokenIt);
                if (token->Type == tokenP.partner && token->Level == level)
                    return token;
            }
        }
    }
	throw new PartnerNotFoundException("Partner not found for: " + std::to_string(type) + " on line " + std::to_string(line_number) + " at position " + std::to_string(line_position));
    
    return token;
}

// Check if the current token is able to have a partner
// @Param
//	type: the function will check if this type can have a partner.
bool Tokenizer::shouldFindPartner(TokenType type)
{
	std::vector<TokenType> types{ TokenType::CloseCurlyBracket, TokenType::CloseMethod, TokenType::Else, TokenType::ElseIf };
	return std::find(types.begin(), types.end(), type) != types.end();
}

// This function will read a new line from the file.
void Tokenizer::nextLine()
{
	while (std::getline(file, line_remaining))
	{
		++line_number;
		line_position = 1;

		if (line_remaining.length() > 0)
			break;
	}
}

std::list<Token>* Tokenizer::GetTokenList()
{
    return token_list;
}

// From Stackoverflow http://stackoverflow.com/a/217605
// trim from start
std::string Tokenizer::ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

// trim from end
std::string Tokenizer::rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}

// trim from both ends
std::string Tokenizer::trim(std::string &s) {
    std::string temp = rtrim(s);
    return ltrim(temp);
}

Tokenizer::~Tokenizer()
{
	file.close();
	delete token_list;
	token_list = nullptr;
}