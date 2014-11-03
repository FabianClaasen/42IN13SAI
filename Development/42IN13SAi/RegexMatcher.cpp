#include "RegexMatcher.h"

RegexMatcher::RegexMatcher()
{}

RegexMatcher::RegexMatcher(std::string regexString) : Regex("^(" + regexString + ")", boost::regex_constants::optimize)
{
}

RegexMatcher::RegexMatcher(const RegexMatcher& other) :Regex(other.Regex)
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