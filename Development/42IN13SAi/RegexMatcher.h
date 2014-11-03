#pragma once

#include <boost/regex.hpp>
#include <string>

class RegexMatcher
{
private:
    boost::regex Regex;
    
public:
	RegexMatcher();
    RegexMatcher(const RegexMatcher& other);
    RegexMatcher(std::string regexString);
    int Match(std::string text);
};