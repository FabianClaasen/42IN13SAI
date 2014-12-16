///
/// @file RegexMatcher.h
/// @brief Contains the matching function
/// @ingroup Tokenizer
///
/// Does the actual matching of a string against the given regex
/// for this it makes use of the Boost regex. Boost Regex gives better performance
///
#pragma once

#include <boost/regex.hpp>
#include <string>

class RegexMatcher
{
private:
    /// The Boost regex
    ///
    boost::regex Regex;
    
    /// The regex string for matching
    ///
	std::string regexString;
    
public:
    /// The default constructor
    ///
	RegexMatcher();
    
    /// The copy constructor
    /// @param other The RegexMatcher to copy
    ///
    RegexMatcher(const RegexMatcher& other);
    
    /// The preferred constructor taking the regex string
    ///
    /// @param regexString The string containing the regex
    ///
    RegexMatcher(std::string regexString);
    
    /// @brief The matching function
    ///
    /// Matches the given string. It does a search, his to make sure
    /// that only the first match will be used. It returns the length
    /// of the matched text so the tokenizer can substring
    ///
    /// @param text The string to match
    /// @return int The length of the matched text
    ///
    int Match(std::string text);
    
    /// Returns the regex string used for matchig
    ///
    /// @return The regex string used for matching
    ///
	std::string GetRegexString();
};