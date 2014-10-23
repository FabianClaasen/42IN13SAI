#include <stdio.h>

#include "Grammar.h"

std::list<TokenDefinition> Grammar::getGrammar()
{
    std::list<TokenDefinition> definitions;
    definitions.push_back(TokenDefinition("var\\b", TokenType::Var));
    
    // Keywords
    definitions.push_back(TokenDefinition("else if\\b", TokenType::ElseIf));
    definitions.push_back(TokenDefinition("if\\b", TokenType::If));
    definitions.push_back(TokenDefinition("else\\b", TokenType::Else));
    definitions.push_back(TokenDefinition("while\\b", TokenType::While));
    definitions.push_back(TokenDefinition("frl\\b", TokenType::ForLoop));

    // functions
    definitions.push_back(TokenDefinition("func\\b", TokenType::KeyIdentifier));
    definitions.push_back(TokenDefinition("\\|", TokenType::Seperator));

    //internal functions
    definitions.push_back(TokenDefinition("prnt\\b", TokenType::PrintLine));
    definitions.push_back(TokenDefinition("stop\\b", TokenType::Stop));
    definitions.push_back(TokenDefinition("sqr\\b", TokenType::Square));
    definitions.push_back(TokenDefinition("sqrt\\b", TokenType::SquareRoot));
    definitions.push_back(TokenDefinition("cbrt\\b", TokenType::CubeRoot));
    definitions.push_back(TokenDefinition("sin\\b", TokenType::Sine));
    definitions.push_back(TokenDefinition("cos\\b", TokenType::Cosine));
    definitions.push_back(TokenDefinition("tan\\b", TokenType::Tangent));
    definitions.push_back(TokenDefinition("deg\\b", TokenType::Degree));
    definitions.push_back(TokenDefinition("rad\\b", TokenType::Radiant));
    definitions.push_back(TokenDefinition("perc\\b", TokenType::Percent));
    definitions.push_back(TokenDefinition("prom\\b", TokenType::PerMillage));
    definitions.push_back(TokenDefinition("log\\b", TokenType::NormalLog));
    definitions.push_back(TokenDefinition("ln\\b", TokenType::Nlog));
    definitions.push_back(TokenDefinition("mod\\b", TokenType::Modulo));
    
    // return
    definitions.push_back(TokenDefinition("ret\\b", TokenType::Return));
    
    // return types
    definitions.push_back(TokenDefinition("void\\b", TokenType::Void));
    definitions.push_back(TokenDefinition("float", TokenType::FloatReturn));
    
    // types
    definitions.push_back(TokenDefinition("[0-9]{1,12}(?:\\.[0-9]{1,12}|(?:e|E)-?[1-9]{1,3})?", TokenType::Float));
    
    // comparators
    definitions.push_back(TokenDefinition(">=", TokenType::GreaterOrEqThan));
    definitions.push_back(TokenDefinition("<=", TokenType::LowerOrEqThan));
    definitions.push_back(TokenDefinition(">", TokenType::GreaterThan));
    definitions.push_back(TokenDefinition("<", TokenType::LowerThan));
    definitions.push_back(TokenDefinition("=", TokenType::Equals));
    definitions.push_back(TokenDefinition("==|!=", TokenType::Comparator));
    
    // operators
    definitions.push_back(TokenDefinition("\\+{2}", TokenType::UniOperatorPlus));
    definitions.push_back(TokenDefinition("[-]{2}", TokenType::UniOperatorMinus));
    definitions.push_back(TokenDefinition("\\+(?!\\+)", TokenType::OperatorPlus));
    definitions.push_back(TokenDefinition("-(?!-)", TokenType::OperatorMinus));
    definitions.push_back(TokenDefinition("\\*", TokenType::OperatorMultiply));
    definitions.push_back(TokenDefinition("\\/", TokenType::OperatorDivide));
    definitions.push_back(TokenDefinition("\\^", TokenType::OperatorRaised));
    
    // conditions
    definitions.push_back(TokenDefinition("&&", TokenType::And));
    definitions.push_back(TokenDefinition("\\||", TokenType::Or));
    
    // bracket types
    definitions.push_back(TokenDefinition("\\[", TokenType::OpenMethod));
    definitions.push_back(TokenDefinition("\\]", TokenType::CloseMethod));
    definitions.push_back(TokenDefinition("\\{", TokenType::OpenCurlyBracket));
    definitions.push_back(TokenDefinition("\\}", TokenType::CloseCurlyBracket));
    definitions.push_back(TokenDefinition("\\(", TokenType::OpenBracket));
    definitions.push_back(TokenDefinition("\\)", TokenType::CloseBracket));

    definitions.push_back(TokenDefinition("[#!@$%&*\"\\.]", TokenType::Special));
    definitions.push_back(TokenDefinition("_?[a-zA-Z][a-zA-Z0-9_]*", TokenType::Identifier));
    
    definitions.push_back(TokenDefinition("\\;", TokenType::EOL));
    
    return definitions;
}

std::vector<TokenPartner> Grammar::getPartners()
{
    std::vector<TokenPartner> partners;
    partners.push_back(TokenPartner(TokenType::Else, TokenType::ElseIf));
    partners.push_back(TokenPartner(TokenType::Else, TokenType::If));
    partners.push_back(TokenPartner(TokenType::ElseIf, TokenType::Else));
    partners.push_back(TokenPartner(TokenType::ElseIf, TokenType::If));
    partners.push_back(TokenPartner(TokenType::CloseBracket, TokenType::OpenBracket));
    partners.push_back(TokenPartner(TokenType::CloseMethod, TokenType::OpenMethod));
    partners.push_back(TokenPartner(TokenType::CloseCurlyBracket, TokenType::OpenCurlyBracket));
    
    return partners;
}