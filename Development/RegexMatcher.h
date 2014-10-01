//
//  RegexMatcher.h
//  tokenizer
//
//  Created by Alex Nagelkerke on 19-09-14.
//  Copyright (c) 2014 Alex Nagelkerke. All rights reserved.
//
#ifndef REGEXMATCHER_H_INCLUDED
#define REGEXMATCHER_H_INCLUDED

#include <regex>
#include <string>

class RegexMatcher
{
private:
    std::regex Regex;
    
public:
    RegexMatcher(std::string regexString);
    int Match(std::string &text);
};

#endif