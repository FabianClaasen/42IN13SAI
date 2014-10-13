#pragma once

#include <list>

#include "TokenDefinition.h"
#include "TokenPartner.h"

class Grammar
{
public:
    static std::list<TokenDefinition> getGrammar();
    static std::list<TokenPartner> getPartners();
};