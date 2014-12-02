#include "RegexMatcher.h"

RegexMatcher::RegexMatcher()
{}

RegexMatcher::RegexMatcher(std::string regexstring) : Regex("^(" + regexstring + ")", boost::regex_constants::optimize), regexString(regexstring)
{
}

RegexMatcher::RegexMatcher(const RegexMatcher& other) : Regex(other.Regex), regexString(other.regexString)
{
}

int RegexMatcher::Match(std::string text)
{
    boost::cmatch item;
    if (!boost::regex_search(text.c_str(), item, Regex))
        return 0;
    else
        return (int)item.length();
}

std::string RegexMatcher::GetRegexString()
{
	return regexString;
}