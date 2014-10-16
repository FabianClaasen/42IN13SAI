#pragma once

#include <boost/regex.hpp>
#include <string>

class RegexMatcher
{
private:
    const boost::regex Regex;
    
public:
    RegexMatcher(std::string regexString);
    int Match(std::string &text);
};