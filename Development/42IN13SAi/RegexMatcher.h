//
//  RegexMatcher.h
//  tokenizer
//
#pragma once

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