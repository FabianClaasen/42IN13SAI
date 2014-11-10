//
//  Grammar.cpp
//  tokenizer
//
//  Created by Alex Nagelkerke on 20-09-14.
//  Copyright (c) 2014 Alex Nagelkerke. All rights reserved.
//

#include <stdio.h>
#include "Grammar.h"

std::list<TokenDefinition> Grammar::getGrammar()
{
    std::list<TokenDefinition> *definitions = new std::list<TokenDefinition>();
    
    // Keywords
    definitions->push_back(*new TokenDefinition("if", TokenType::If));
    definitions->push_back(*new TokenDefinition("else", TokenType::Else));
    definitions->push_back(*new TokenDefinition("else if", TokenType::ElseIf));
    definitions->push_back(*new TokenDefinition("while", TokenType::While));
    definitions->push_back(*new TokenDefinition("frl", TokenType::ForLoop));
    
    definitions->push_back(*new TokenDefinition("func", TokenType::KeyIdentifier));
    definitions->push_back(*new TokenDefinition("(sin|cos|exp|ln|sqrt)\b", TokenType::Function));
    
    // return
    definitions->push_back(*new TokenDefinition("ret", TokenType::Return));
    
    // types
    definitions->push_back(*new TokenDefinition("yes|no", TokenType::Boolean));
    definitions->push_back(*new TokenDefinition("[0-9]{1,12}(?:\\.[0-9]{1,12}|(?:e|E)-?[1-9]{1,3})", TokenType::Double));
    definitions->push_back(*new TokenDefinition("[0-9]+", TokenType::Integer));
    
    //specials
    definitions->push_back(*new TokenDefinition(">=", TokenType::GreaterOrEqThan));
    definitions->push_back(*new TokenDefinition("<=", TokenType::LowerOrEqThan));
    definitions->push_back(*new TokenDefinition(">", TokenType::GreaterThan));
    definitions->push_back(*new TokenDefinition("<", TokenType::LowerThan));
    definitions->push_back(*new TokenDefinition("=", TokenType::Equals));
    definitions->push_back(*new TokenDefinition("\\+{2}", TokenType::UniOperatorPlus));
    definitions->push_back(*new TokenDefinition("[-]{2}", TokenType::UniOperatorMinus));
    definitions->push_back(*new TokenDefinition("\\+(?!\\+)", TokenType::OperatorPlus));
    definitions->push_back(*new TokenDefinition("-(?!-)", TokenType::OperatorMinus));
    definitions->push_back(*new TokenDefinition("\\*", TokenType::OperatorMultiply));
    definitions->push_back(*new TokenDefinition("\\/", TokenType::OperatorDivide));
    definitions->push_back(*new TokenDefinition("\\^", TokenType::OperatorRaised));
    definitions->push_back(*new TokenDefinition("\\|\\||&&|==|!=", TokenType::Comparator));
    definitions->push_back(*new TokenDefinition("[#!@$%&*\"\\.]", TokenType::Special));
    definitions->push_back(*new TokenDefinition("::", TokenType::Special));
    definitions->push_back(*new TokenDefinition("\\[", TokenType::OpenMethod));
    definitions->push_back(*new TokenDefinition("\\]", TokenType::CloseMethod));
    definitions->push_back(*new TokenDefinition("\\{", TokenType::OpenCurlyBracket));
    definitions->push_back(*new TokenDefinition("\\}", TokenType::CloseCurlyBracket));
    definitions->push_back(*new TokenDefinition("\\(", TokenType::OpenBracket));
    definitions->push_back(*new TokenDefinition("\\)", TokenType::CloseBracket));
    
    definitions->push_back(*new TokenDefinition("[a-zA-Z][a-zA-Z0-9_]*", TokenType::Identifier));
    
    definitions->push_back(*new TokenDefinition("\\;", TokenType::EOL));
    
    return *definitions;
}

std::list<TokenPartner> Grammar::getPartners()
{
    std::list<TokenPartner> *partners = new std::list<TokenPartner>();
    partners->push_back(*new TokenPartner(TokenType::Else, TokenType::ElseIf));
    partners->push_back(*new TokenPartner(TokenType::Else, TokenType::If));
    partners->push_back(*new TokenPartner(TokenType::ElseIf, TokenType::Else));
    partners->push_back(*new TokenPartner(TokenType::ElseIf, TokenType::If));
    partners->push_back(*new TokenPartner(TokenType::CloseBracket, TokenType::OpenBracket));
    partners->push_back(*new TokenPartner(TokenType::CloseMethod, TokenType::OpenMethod));
    
    return *partners;
}