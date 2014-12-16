///
/// @file Grammar.h
/// @brief Contains all the grammar for Short-C
/// @ingroup Tokenizer
///
/// Contains two maps with the main grammar of the language and
/// the partners tokens can have.
///
#pragma once

#include <vector>
#include <list>

#include "TokenDefinition.h"
#include "TokenPartner.h"

class Grammar
{
public:
    /// The list containing the TokenDefinition's
    ///
    static std::list<TokenDefinition> getGrammar();
    
    /// The vector containing the TokenPartner's
    ///
    static std::vector<TokenPartner> getPartners();
};