#include <stdio.h>

#include "Grammar.h"

std::list<TokenDefinition> Grammar::getGrammar()
{
    std::list<TokenDefinition> definitions;
    
    // Keywords
    definitions.push_back(TokenDefinition("if", TokenType::If));
    definitions.push_back(TokenDefinition("else", TokenType::Else));
    definitions.push_back(TokenDefinition("else if", TokenType::ElseIf));
    definitions.push_back(TokenDefinition("while", TokenType::While));
    definitions.push_back(TokenDefinition("frl", TokenType::ForLoop));
    
    definitions.push_back(TokenDefinition("func", TokenType::KeyIdentifier));
    definitions.push_back(TokenDefinition("(sin|cos|exp|ln|sqrt)\b", TokenType::Function));
    
    // return
    definitions.push_back(TokenDefinition("ret", TokenType::Return));
    
    // types
    definitions.push_back(TokenDefinition("[0-9]{1,12}(?:\\.[0-9]{1,12}|(?:e|E)-?[1-9]{1,3})?", TokenType::Float));
    
    //specials
    definitions.push_back(TokenDefinition(">=", TokenType::GreaterOrEqThan));
    definitions.push_back(TokenDefinition("<=", TokenType::LowerOrEqThan));
    definitions.push_back(TokenDefinition(">", TokenType::GreaterThan));
    definitions.push_back(TokenDefinition("<", TokenType::LowerThan));
    definitions.push_back(TokenDefinition("=", TokenType::Equals));
    definitions.push_back(TokenDefinition("\\+{2}", TokenType::UniOperatorPlus));
    definitions.push_back(TokenDefinition("[-]{2}", TokenType::UniOperatorMinus));
    definitions.push_back(TokenDefinition("\\+(?!\\+)", TokenType::OperatorPlus));
    definitions.push_back(TokenDefinition("-(?!-)", TokenType::OperatorMinus));
    definitions.push_back(TokenDefinition("\\*", TokenType::OperatorMultiply));
    definitions.push_back(TokenDefinition("\\/", TokenType::OperatorDivide));
    definitions.push_back(TokenDefinition("\\^", TokenType::OperatorRaised));
    definitions.push_back(TokenDefinition("\\|==|!=", TokenType::Comparator));
    definitions.push_back(TokenDefinition("[#!@$%&*\"\\.]", TokenType::Special));
    definitions.push_back(TokenDefinition("::", TokenType::Special));
    definitions.push_back(TokenDefinition("\\[", TokenType::OpenMethod));
    definitions.push_back(TokenDefinition("\\]", TokenType::CloseMethod));
    definitions.push_back(TokenDefinition("\\{", TokenType::OpenCurlyBracket));
    definitions.push_back(TokenDefinition("\\}", TokenType::CloseCurlyBracket));
    definitions.push_back(TokenDefinition("\\(", TokenType::OpenBracket));
    definitions.push_back(TokenDefinition("\\)", TokenType::CloseBracket));
	definitions.push_back(TokenDefinition("&&", TokenType::And));
	definitions.push_back(TokenDefinition("\\||", TokenType::Or));

    definitions.push_back(TokenDefinition("[a-zA-Z][a-zA-Z0-9_]*", TokenType::Identifier));
    
    definitions.push_back(TokenDefinition("\\;", TokenType::EOL));
    
    return definitions;
}

std::list<TokenPartner> Grammar::getPartners()
{
    std::list<TokenPartner> partners;
    partners.push_back(TokenPartner(TokenType::Else, TokenType::ElseIf));
    partners.push_back(TokenPartner(TokenType::Else, TokenType::If));
    partners.push_back(TokenPartner(TokenType::ElseIf, TokenType::Else));
    partners.push_back(TokenPartner(TokenType::ElseIf, TokenType::If));
    partners.push_back(TokenPartner(TokenType::CloseBracket, TokenType::OpenBracket));
    partners.push_back(TokenPartner(TokenType::CloseMethod, TokenType::OpenMethod));
    
    return partners;
}