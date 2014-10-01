//
//  Grammar.h
//  tokenizer
//
//  Created by Alex Nagelkerke on 20-09-14.
//  Copyright (c) 2014 Alex Nagelkerke. All rights reserved.
//

#include <list>
#include "TokenDefinition.h"
#include "TokenPartner.h"

class Grammar
{
public:
    static std::list<TokenDefinition> getGrammar();
    static std::list<TokenPartner> getPartners();
};