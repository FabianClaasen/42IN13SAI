#include "Compiler.h"
#include "CompilerNode.h"
#include "Token.h"

#include <iterator>
#include <string>
#include <list>

/*
 Check what the next token is
 */
Token* Compiler::peekNext()
{
    std::list<Token>::iterator it = tokenizerTokens.begin();
    std::advance(it, currentToken + 1);
    
    return &*it;
}

/*
 Get the next token
 */
Token Compiler::getNext()
{
    std::list<Token>::iterator it = tokenizerTokens.begin();
    
    if (Compiler::peekNext() != nullptr)
    {
        std::advance(it, ++currentToken);
    }
    else
    {
        throw std::string("Token missing");
    }
    
    return *it;
}

void Compiler::match(TokenType type)
{
    if (Compiler::peekNext() == nullptr)
    {
        throw std::string(&"Expected: "[type]);
    }
    
    Token currentToken = getNext();
    
    if (currentToken.Type != type)
    {
        throw std::string(&"Expected: "[type]);
    }
}

void Compiler::parseExpression()
{
    //TODO might need to return something
}

/*
 Also check and parse if-else statement
 */
void Compiler::parseIfStatement()
{
    Token currentToken = getNext();
    bool hasPartner = false;
    
    if (currentToken.Type == TokenType::If)
    {
        if (currentToken.Partner != nullptr)
        {
            hasPartner = true;
        }
    }
    else
    {
        throw std::string("Expected if keyword");
    }
    
    match(TokenType::OpenBracket);
    
    parseExpression();
    
    match(TokenType::CloseBracket);
    match(TokenType::OpenCurlyBracket);
    
    while ((currentToken = getNext()).Type != TokenType::CloseCurlyBracket)
    {
        //TODO parse stuff in if statement
    }
    
    if (hasPartner)
    {
        //TODO parse else statement
    }
}

/*
 Parse while and for loops
 */
void Compiler::parseLoopStatement()
{
    
}

/*
 Return the type of the value from a token
 */
string Compiler::getTokenValueType(Token currentToken)
{
    if (currentToken.Type == TokenType::Boolean)
    {
        return "Boolean";
    }
    else if (currentToken.Type == TokenType::Integer)
    {
        return "Integer";
    }
    else if (currentToken.Type == TokenType::Double)
    {
        return "Double";
    }
    else
    {
        return "String";
    }
}

/*
 Also parse (standard) Arithmetical operations
 */
void Compiler::parseAssignmentStatement()
{
    std::string expression;
    std::string identifier;
    std::string value;
    
    Token currentToken = getNext();
    if (currentToken.Type == TokenType::Identifier)
    {
        identifier = currentToken.Value;
    }
    else
    {
        throw std::string("Identifier expected");
    }
    
    currentToken = getNext();
    
    //TODO Arithmetical operations
    
    if (currentToken.Type == TokenType::Equals)
    {
        expression = "$assignment";
    }
    
    currentToken = getNext();
    
    value = currentToken.Value;
    expression = expression + getTokenValueType(currentToken);
}

/*
 Check what to parse
 */
void Compiler::parseStatement()
{
    switch (peekNext()->Type)
    {
        case TokenType::If:
            parseIfStatement();
            break;
        case TokenType::While:
            parseLoopStatement();
            break;
        case TokenType::Identifier:
            parseAssignmentStatement();
            break;
        default:
            throw std::string("No statement found");
            break;
    }
}

/*
 keep parsing as long as there are tokens
 */
void Compiler::compile()
{
    while (Compiler::peekNext() != nullptr)
    {
        parseStatement();
    }
}

/*
 Constructor
 */
Compiler::Compiler(std::list<Token> tokens)
{
    tokenizerTokens = tokens;
    
    compile();
}

/*
 Destructor
 */
Compiler::~Compiler()
{
}
