#include <stdio.h>

#include "Grammar.h"

std::list<TokenDefinition> Grammar::getGrammar()
{
    std::list<TokenDefinition> definitions;
    definitions.push_back(TokenDefinition("var\\b", MyTokenType::Var));
    
    // Keywords
    definitions.push_back(TokenDefinition("else if\\b", MyTokenType::ElseIf));
    definitions.push_back(TokenDefinition("if\\b", MyTokenType::If));
    definitions.push_back(TokenDefinition("else\\b", MyTokenType::Else));
    definitions.push_back(TokenDefinition("while\\b", MyTokenType::While));
    definitions.push_back(TokenDefinition("frl\\b", MyTokenType::ForLoop));

    // functions
    definitions.push_back(TokenDefinition("func\\sMain\\b", MyTokenType::MainFunction));
    definitions.push_back(TokenDefinition("func\\b", MyTokenType::Function));
    definitions.push_back(TokenDefinition("\\|", MyTokenType::Seperator));

    //internal functions
    definitions.push_back(TokenDefinition("prnt\\b", MyTokenType::PrintLine));
    definitions.push_back(TokenDefinition("stop\\b", MyTokenType::Stop));
    definitions.push_back(TokenDefinition("sqr\\b", MyTokenType::Square));
    definitions.push_back(TokenDefinition("sqrt\\b", MyTokenType::SquareRoot));
    definitions.push_back(TokenDefinition("cbrt\\b", MyTokenType::CubeRoot));
    definitions.push_back(TokenDefinition("sin\\b", MyTokenType::Sine));
    definitions.push_back(TokenDefinition("cos\\b", MyTokenType::Cosine));
    definitions.push_back(TokenDefinition("tan\\b", MyTokenType::Tangent));
    definitions.push_back(TokenDefinition("deg\\b", MyTokenType::Degree));
    definitions.push_back(TokenDefinition("rad\\b", MyTokenType::Radiant));
    definitions.push_back(TokenDefinition("perc\\b", MyTokenType::Percent));
    definitions.push_back(TokenDefinition("prom\\b", MyTokenType::PerMillage));
    definitions.push_back(TokenDefinition("log\\b", MyTokenType::NormalLog));
    definitions.push_back(TokenDefinition("ln\\b", MyTokenType::Nlog));
    definitions.push_back(TokenDefinition("mod\\b", MyTokenType::Modulo));
    
    // return
    definitions.push_back(TokenDefinition("ret\\b", MyTokenType::Return));
    
    // return types
    definitions.push_back(TokenDefinition("void\\b", MyTokenType::Void));
    definitions.push_back(TokenDefinition("float\\b", MyTokenType::FloatReturn));
    
    // types
    definitions.push_back(TokenDefinition("[0-9]{1,12}(?:\\.[0-9]{1,12}|(?:e|E)-?[1-9]{1,3})?", MyTokenType::Float));
    
    // comparators
    definitions.push_back(TokenDefinition(">=", MyTokenType::GreaterOrEqThan));
    definitions.push_back(TokenDefinition("<=", MyTokenType::LowerOrEqThan));
    definitions.push_back(TokenDefinition(">", MyTokenType::GreaterThan));
    definitions.push_back(TokenDefinition("<", MyTokenType::LowerThan));
    definitions.push_back(TokenDefinition("==|!=", MyTokenType::Comparator));
    definitions.push_back(TokenDefinition("=", MyTokenType::Equals));
    
    // operators
    definitions.push_back(TokenDefinition("\\+{2}", MyTokenType::UniOperatorPlus));
    definitions.push_back(TokenDefinition("[-]{2}", MyTokenType::UniOperatorMinus));
    definitions.push_back(TokenDefinition("\\+(?!\\+)", MyTokenType::OperatorPlus));
    definitions.push_back(TokenDefinition("-(?!-)", MyTokenType::OperatorMinus));
    definitions.push_back(TokenDefinition("\\*", MyTokenType::OperatorMultiply));
    definitions.push_back(TokenDefinition("\\/", MyTokenType::OperatorDivide));
    definitions.push_back(TokenDefinition("\\^", MyTokenType::OperatorRaised));
    
    // conditions
    definitions.push_back(TokenDefinition("&&", MyTokenType::And));
    definitions.push_back(TokenDefinition("\\||", MyTokenType::Or));
    
    // bracket types
    definitions.push_back(TokenDefinition("\\[", MyTokenType::OpenMethod));
    definitions.push_back(TokenDefinition("\\]", MyTokenType::CloseMethod));
    definitions.push_back(TokenDefinition("\\{", MyTokenType::OpenCurlyBracket));
    definitions.push_back(TokenDefinition("\\}", MyTokenType::CloseCurlyBracket));
    definitions.push_back(TokenDefinition("\\(", MyTokenType::OpenBracket));
    definitions.push_back(TokenDefinition("\\)", MyTokenType::CloseBracket));

    definitions.push_back(TokenDefinition("[#!@$%&*\"\\.]", MyTokenType::Special));
    definitions.push_back(TokenDefinition("_?[a-zA-Z][a-zA-Z0-9_]*", MyTokenType::Identifier));
    
    definitions.push_back(TokenDefinition("\\;", MyTokenType::EOL));
    
    return definitions;
}

std::vector<TokenPartner> Grammar::getPartners()
{
    std::vector<TokenPartner> partners;
    partners.push_back(TokenPartner(MyTokenType::Else, MyTokenType::ElseIf));
    partners.push_back(TokenPartner(MyTokenType::Else, MyTokenType::If));
    partners.push_back(TokenPartner(MyTokenType::ElseIf, MyTokenType::Else));
    partners.push_back(TokenPartner(MyTokenType::ElseIf, MyTokenType::If));
    partners.push_back(TokenPartner(MyTokenType::CloseBracket, MyTokenType::OpenBracket));
    partners.push_back(TokenPartner(MyTokenType::CloseMethod, MyTokenType::OpenMethod));
    partners.push_back(TokenPartner(MyTokenType::CloseCurlyBracket, MyTokenType::OpenCurlyBracket));
    
    return partners;
}