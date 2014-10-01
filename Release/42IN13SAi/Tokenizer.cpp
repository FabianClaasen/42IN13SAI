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

		std::list<TokenDefinition>::iterator definitionIterator;
		for (definitionIterator = tokenDefinitions.begin(); definitionIterator != tokenDefinitions.end(); ++definitionIterator)
		{
			TokenDefinition definition = *definitionIterator;
			int matched = definition.matcher->Match(lineRemaining);

			if (matched > 0)
			{
				match = true;

				std::string tokenValue = lineRemaining.substr(0, matched);

				if (definition.tokenType == TokenType::OpenBracket || definition.tokenType == TokenType::OpenCurlyBracket || definition.tokenType == TokenType::OpenMethod)
					level++;

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
				Token *token = new Token(lineNumber, linePosition, level, tokenValue, definition.tokenType, partner);

				// if partner found, give this token to partner
				if (partner)
					partner->Partner = token;

				tokenList->push_back(*token);

				if (definition.tokenType == TokenType::CloseBracket || definition.tokenType == TokenType::CloseCurlyBracket || definition.tokenType == TokenType::CloseMethod)
					level--;

				linePosition += matched;
				lineRemaining = lineRemaining.substr(matched);

				if (lineRemaining.length() == 0)
					nextLine();

				break;
			}
		}

		if (!match)
			throw new ParseException("Unrecognized character '" + lineRemaining + "' on line " + std::to_string(lineNumber) + " at position " + std::to_string(linePosition));
	}
}

Token* Tokenizer::findPartner(TokenType &tokenType, int level)
{
    Token *token = nullptr;
    std::list<TokenPartner>::iterator tokenPartner;
    for (tokenPartner = tokenPartners.begin(); tokenPartner != tokenPartners.end(); ++tokenPartner)
    {
        TokenPartner tokenP = *tokenPartner;
        if (tokenP.token == tokenType)
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
    throw new PartnerNotFoundException("Partner not found for: " + std::to_string(tokenType) + " on line " + std::to_string(lineNumber) + " at position " + std::to_string(linePosition));
    
    return token;
}

bool Tokenizer::shouldFindPartner(TokenType type)
{
	std::vector<TokenType> types{ TokenType::CloseCurlyBracket, TokenType::CloseMethod, TokenType::Else, TokenType::ElseIf };
	return std::find(types.begin(), types.end(), type) != types.end();
}

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