///
/// @file TokenizerController.h
/// @brief The controller for the Tokenizer
/// @ingroup Tokenizer
///
/// The controller is the link between the MainController
/// and the Tokenizer. The MainController only has access to this controller
/// It also gets the grammar from the Grammar class
///
#pragma once
#include <list>

#include "Token.h"
#include "Tokenizer.h"
#include "Grammar.h"

class TokenizerController
{
private:
    /// The Tokenizer
    ///
	Tokenizer *tokenizer;
public:
    /// The constructor
    ///
    /// @param filename The path to the file that's going to be tokenized
    ///
	TokenizerController(std::string filename);
	virtual ~TokenizerController();
    
    /// Called to start the actual tokenizing
    ///
	void Tokenize();
    
    /// Called after tokenizing to get the tokenized tokens
    ///
    /// @return The tokenized tokens
    ///
	std::vector<std::shared_ptr<Token>> GetCompilerTokens();

    /// Called to see if exceptions were encountered
    ///
    /// @return Returns true if exceptions were encountered
    bool HasExceptions();
};

