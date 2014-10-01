 //
//  Tokenizer.cpp
//  tokenizer
//
//  Created by Alex Nagelkerke on 19-09-14.
//  Copyright (c) 2014 Alex Nagelkerke. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <cctype>
#include <functional>
#include "Tokenizer.h"

Tokenizer::Tokenizer(std::string fileLocation, std::list<TokenDefinition> definitions, std::list<TokenPartner> partners)
{
    // set defaults
    lineNumber      = 0;
    linePosition    = 1;
    level           = 1;
    lineRemaining   = "";
    
    tokenDefinitions = definitions;
    tokenPartners    = partners;
    
    file.open(fileLocation);
    
    nextLine();
}

// The tokenize function will fill the tokenlist with all the tokens
// gathered from the code send to it.
void Tokenizer::Tokenize()
{
	tokenList = new std::list<Token>();

	while (lineRemaining.length() != 0)
	{
		bool match = false;
		lineRemaining = trim(lineRemaining);
		if (lineRemaining.length() == 0)
		{
			nextLine();
			continue;
		}

		// Loop through all the token definitions
		std::list<TokenDefinition>::iterator definitionIterator;
		for (definitionIterator = tokenDefinitions.begin(); definitionIterator != tokenDefinitions.end(); ++definitionIterator)
		{
			TokenDefinition definition = *definitionIterator;
			int matched = definition.matcher->Match(lineRemaining);

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
				std::string tokenValue = lineRemaining.substr(0, matched);
				Token *token = new Token(lineNumber, linePosition, level, tokenValue, definition.tokenType, partner);
				
				// if partner found, give this token to partner
				if (partner)
					partner->Partner = token;
				tokenList->push_back(*token);

				// Check if the level should be lowered
				if (definition.tokenType == TokenType::CloseBracket || definition.tokenType == TokenType::CloseCurlyBracket || definition.tokenType == TokenType::CloseMethod)
					level--;

				// Change your position and line
				linePosition += matched;
				lineRemaining = lineRemaining.substr(matched);
				if (lineRemaining.length() == 0)
					nextLine();

				break;
			}
		}

		// Throw an exception if the target couldnt be parsed as a token.
		if (!match)
			throw new ParseException("Unrecognized character '" + lineRemaining + "' on line " + std::to_string(lineNumber) + " at position " + std::to_string(linePosition));
	}
}

// Find a partner for the current token.
// @param
//  type: this is the type of the token where you need to find a match for.
//	level: this is the level of the tokentype, the partner needs to be on the same level.
Token* Tokenizer::findPartner(TokenType &type, int level)
{
    Token *token = nullptr;
    std::list<TokenPartner>::iterator tokenPartner;
    for (tokenPartner = tokenPartners.begin(); tokenPartner != tokenPartners.end(); ++tokenPartner)
    {
        TokenPartner tokenP = *tokenPartner;
		if (tokenP.token == type)
        {
            std::list<Token>::reverse_iterator tokenIt;
            for (tokenIt = tokenList->rbegin(); tokenIt != tokenList->rend(); ++tokenIt)
            {
                token = &(*tokenIt);
                if (token->Type == tokenP.partner && token->Level == level)
                    return token;
            }
        }
    }
	throw new PartnerNotFoundException("Partner not found for: " + std::to_string(type) + " on line " + std::to_string(lineNumber) + " at position " + std::to_string(linePosition));
    
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
    while (std::getline(file, lineRemaining))
    {
        ++lineNumber;
        linePosition = 1;
        
        if (lineRemaining.length() > 0)
            break;
    }
}

std::list<Token>* Tokenizer::GetTokenList()
{
    return tokenList;
}

// From Stackoverflow http://stackoverflow.com/a/217605
// doesn't work in VS no idea why
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