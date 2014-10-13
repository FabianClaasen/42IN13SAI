#include <stdio.h>

#include "RegexMatcher.h"

RegexMatcher::RegexMatcher(std::string regexString)
{
    Regex = std::regex("^(" + regexString + ")");
}

int RegexMatcher::Match(std::string &text)
{
    std::match_results<std::string::const_iterator> item;
    if (!std::regex_search(text, item, Regex))
        return 0;
    else
        return (int)item.length();
}