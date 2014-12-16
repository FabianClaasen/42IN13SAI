///
/// @file TokenDefinition.h
/// @brief Contains the type info needed for creating tokens
/// @ingroup Tokenizer
///
/// Contains the RegexMatcher and MyTokenType information for tokens.
/// The Tokenizer uses this to create the tokens for the Compiler
///
#pragma once

#include <string>

#include "RegexMatcher.h"
#include "MyTokenType.h"

class TokenDefinition
{
public:
    TokenDefinition();
    
    /// @brief The preferred constructor
    ///
    /// Takes the regex string to match the source code against and
    /// the MyTokenType for this sort of token. Creates the RegexMatcher
    /// from the given regex string
    ///
    /// @param regexString The regex
    /// @param type The MyTokenType
    ///
    TokenDefinition(std::string regexString, MyTokenType type);
    
    /// The copy constructor
    ///
    /// @param other The TokenDefinition to copy
    ///
    TokenDefinition(const TokenDefinition& other);
    
    /// The destructor
    ///
	~TokenDefinition();
    
    /// The RegexMatcher for this definition
    ///
    RegexMatcher matcher;
    
    /// The MyTokenType for this definition
    ///
    MyTokenType myTokenType;
};