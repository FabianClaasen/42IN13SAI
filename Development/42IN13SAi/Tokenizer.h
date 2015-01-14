/// @defgroup Tokenizer The Tokenizer module
/// Contains all classes involved with tokenizing input
///

/// @file Tokenizer.h
/// @brief Used for tokenizing the input short-c source code
/// @ingroup Tokenizer
///
/// Creates tokens from the input short-c. Goes through all the possible
/// MyTokenTypes to create tokens for the Compiler.
///

#pragma once

#include <boost/algorithm/string.hpp>
#include <fstream>
#include <list>
#include <vector>
#include <string>
#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <cctype>
#include <functional>
#include <iostream>

#include "TokenDefinition.h"
#include "Token.h"
#include "TokenPartner.h"
#include "PartnerNotFoundException.h"
#include "ParseException.h"
#include "ExceptionEnum.h"
#include "DiagnosticsBuilder.h"

class Tokenizer
{
public:
    /// @brief The constructor that takes the file path, the token definitions
    ///         and token partners
    ///
    /// The Constructor takes the file path where the IDE stored the given
    /// source code. The Token definitions, which is the grammar it will
    /// check the source code with. The partners are the tokens that need a
    /// partner.
    ///
    /// @param fileLocation The path to the source code
    /// @param definitions The token definitions
    /// @param partners The token partners
    ///
	Tokenizer(std::string fileLocation, std::list<TokenDefinition> definitions, std::vector<TokenPartner> partners);
    
    /// @brief The destructor for the tokenizer
    ///
	virtual ~Tokenizer();

    /// @brief The function that is called when the tokens will be tokenized
    ///
    /// This function actually tokenizes the given source code. It does this
    /// by using the given token definitions, these contain a regex matcher.
    /// \b The regex matcher will match a given line, if there is a match it creates
    /// a new Token with the information the Compiler/Parser will need. It also
    /// gives the tokens a partner when needed, e.g. an "else" token needs an "if"
    /// token.
    ///
	void Tokenize();
    
    /// @brief returns a vector of tokens that were tokenized
    ///
    /// @return The tokenized tokens
    ///
	std::vector<std::shared_ptr<Token>> GetTokenList();
    
    /// @brief returns true if the Tokenizer has encountered any exceptions.
    ///         The MainController then knows to stop
    ///
    /// @return True if there are exceptions
    ///
    bool HasExceptions();
private:
    /// The boolean keeping track of if there are any exceptions
    ///
    bool hasExceptions = false;
    
    /// The current line number of the file being tokenized
    ///
	int lineNumber;
    
    /// The current line position of the line being tokenized
    ///
	int linePosition;
    
    /// @brief The current token level
    ///
    /// The level will get higher when the code goes deeper. I.e. if a '{' bracket
    /// is tokenized the level will go higher, if the closing '}' bracket is
    /// tokenized the level will get lower again. The level starts at 0 for
    /// each file
    ///
	int level;
    
    /// @brief The line remaining for tokenizing
    ///
    /// Is the line of the file containing the text that will be tokenized
    ///
	std::string lineRemaining;
    
    /// The file being currently tokenized
    ///
    std::ifstream file;
    
    /// The list containing the token definitions
    /// @see TokenDefinition.h
    ///
    std::list<TokenDefinition> tokenDefinitions;
    
    /// The vector containing the token partners
    /// @see TokenPartner.h
    ///
    std::vector<TokenPartner> tokenPartners;
    
    /// The vector containing all the tokenized tokens
    /// @see Token.h
    ///
    std::vector<std::shared_ptr<Token>> tokenVector;
    
    /// The function to get the next line of the current file
    ///
    void NextLine();
    
    /// @brief Finds the partner for the given token type
    ///
    /// The function loops through all the tokenized tokens in reverse.
    /// This function can only link the ']' bracket to the '[' bracket. Not the
    /// other way around. TryFindPartner is the function that does that.
    ///
    /// @param type The MyTokenType it should find a partner for
    /// @param level The level the partner must have
    /// @return The Token it should be partnered to
    ///
    std::shared_ptr<Token> FindPartner(MyTokenType &type, int level);
    
    /// @brief Returns if this type needs a partner
    ///
    /// Contains a vector with all the types it should find a partner for.
    /// Using find it is decided if the partner is in the vector
    ///
    /// @param type The MyTokenType
    /// @return Returns true if it should search for a partner
    ///
	bool ShouldFindPartner(MyTokenType &type);
    
    /// @brief Returns if this type needs a partner
    ///
    /// Contains the vector with MyTokenType's that should have a partner
    /// if the tokenizing is done and it's looping over all the tokens one
    /// last time to check all the partners. Uses same logic as ShouldFindPartner
    ///
    /// @param type The MyTokenType
    /// @return Returns true if a partner should be found
    ///
	bool ShouldFindPartnerR(MyTokenType &type);
    
    /// @brief Iterates through all the tokens to find a partner
    ///
    /// After tokenizing all the tokens goes through them to check if
    /// the tokens that need a partner have a partner
    ///
    /// @param token The Token a partner should be found for
    ///
	void TryFindPartner(std::shared_ptr<Token> token);
    
    /// @brief Iterates through all the tokens to find a partner
    ///
    /// After tokenizing all the tokens goes through them to check if
    /// the tokens that need a partner have a partner
    ///
	void CheckClosingPartners();
    
    /// Stringifies a MyTokenType to a humanreadable token
    ///
    /// @param type The MyTokenType to stringify
    ///
	std::string TokenToString(MyTokenType type);
    
    /// Returns a DiagnosticBuilder for the exception occuring
    ///
    /// @see ExceptionEnum.h
    /// @param token The Token the exception happened on
    /// @param exception The exception that happened
    ///
    DiagnosticBuilder Diag(ExceptionEnum exception);
};