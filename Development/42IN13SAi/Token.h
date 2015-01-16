///
/// @file Token.h
/// @brief Contains the token data
/// @ingroup Tokenizer
///
/// Contains all the data the Compiler will need to parse
/// the given source code to a CompilerNode.
///
#pragma once
#include <memory>
#include <string>
#include "MyTokenType.h"


class Token
{
public:
    /// The line number the token is on
    ///
	int LineNumber;
    
    /// The line position the token starts
    ///
	int LinePosition;
    
    /// @brief The level of the token
    ///
    /// The level shows the depth of the token.
    /// The higher the level the deeper it is in a function.
    ///
	int Level;
    
    /// The actual value of the token
    ///
	std::string Value;
    
    /// The MyTokenType type of the token
    ///
	MyTokenType Type;
    
    /// If the token has a partner this will contain it
    ///
	std::weak_ptr<Token> Partner;
    
    /// Sets all the data for the token
    ///
    /// @param lineNumber The line number of the token
    /// @param linePosition The line position of the token
    /// @param value The string value of the token
    /// @param myTokenType The MyTokenType of the token
    /// @param partner The partner of the token, can be nullptr
    ///
	Token(int lineNumber, int linePosition, int level, std::string value, MyTokenType myTokenType, std::shared_ptr<Token> partner);
    
    /// The copy constructor
    /// @param other The Token to copy
    ///
    Token(const Token &other);
    
    /// The default constructor
    ///
	Token();
    
    /// The destructor
    ///
	~Token();
};