#include <stdio.h>

#include "Grammar.h"

std::list<TokenDefinition> Grammar::getGrammar()
{
    std::list<TokenDefinition> definitions;
    definitions.push_back(TokenDefinition("var\\b", MyTokenType::Var));
	definitions.push_back(TokenDefinition("\\{\\}\\b", MyTokenType::Array));
    
    // Keywords
    definitions.push_back(TokenDefinition("\\belse if\\b", MyTokenType::ElseIf));
    definitions.push_back(TokenDefinition("\\bif\\b", MyTokenType::If));
    definitions.push_back(TokenDefinition("\\belse\\b", MyTokenType::Else));
    definitions.push_back(TokenDefinition("\\bwhile\\b", MyTokenType::While));
    definitions.push_back(TokenDefinition("\\bfrl\\b", MyTokenType::ForLoop));

    // functions
    definitions.push_back(TokenDefinition("\\bfunc\\sMain\\b", MyTokenType::MainFunction));
    definitions.push_back(TokenDefinition("\\bfunc\\b", MyTokenType::Function));
    definitions.push_back(TokenDefinition("\\|", MyTokenType::Separator));

    //internal functions
    definitions.push_back(TokenDefinition("\\bprnt\\b", MyTokenType::PrintLine));
    definitions.push_back(TokenDefinition("\\bstop\\b", MyTokenType::Stop));
    definitions.push_back(TokenDefinition("\\bsqr\\b", MyTokenType::Square));
	definitions.push_back(TokenDefinition("\\bcbc\\b", MyTokenType::Cube));
	definitions.push_back(TokenDefinition("\\bpow\\b", MyTokenType::Power));
    definitions.push_back(TokenDefinition("\\bsqrt\\b", MyTokenType::SquareRoot));
    definitions.push_back(TokenDefinition("\\bcbrt\\b", MyTokenType::CubeRoot));
    definitions.push_back(TokenDefinition("\\bsin\\b", MyTokenType::Sine));
    definitions.push_back(TokenDefinition("\\bcos\\b", MyTokenType::Cosine));
    definitions.push_back(TokenDefinition("\\btan\\b", MyTokenType::Tangent));
    definitions.push_back(TokenDefinition("\\bdeg\\b", MyTokenType::Degree));
    definitions.push_back(TokenDefinition("\\brad\\b", MyTokenType::Radiant));
    definitions.push_back(TokenDefinition("\\bperc\\b", MyTokenType::Percent));
    definitions.push_back(TokenDefinition("\\bperm\\b", MyTokenType::Permillage));
    definitions.push_back(TokenDefinition("\\blog10\\b", MyTokenType::CommonLog));
	definitions.push_back(TokenDefinition("\\blog2\\b", MyTokenType::BinaryLog));
	definitions.push_back(TokenDefinition("\\blog\\b", MyTokenType::Log));
    definitions.push_back(TokenDefinition("\\bln\\b", MyTokenType::NaturalLog));
    definitions.push_back(TokenDefinition("\\bmod\\b", MyTokenType::Modulo));
	definitions.push_back(TokenDefinition("\\bgcd\\b", MyTokenType::Gcd));
	definitions.push_back(TokenDefinition("\\bdis\\b", MyTokenType::Discriminant));
	definitions.push_back(TokenDefinition("\\babc\\b", MyTokenType::AbcFormula));
    
    // return
    definitions.push_back(TokenDefinition("\\bret\\b", MyTokenType::Return));
    
    // return types
    definitions.push_back(TokenDefinition("\\bvoid\\b", MyTokenType::Void));
    definitions.push_back(TokenDefinition("\\bfloat\\b", MyTokenType::FloatReturn));
    
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