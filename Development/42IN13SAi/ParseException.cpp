//
//  ParseException.cpp
//  tokenizer
//
#include <stdio.h>
#include "ParseException.h"

ParseException::ParseException(std::string m) : message(m)
{}


const char* ParseException::what() const throw()
{
    return message.c_str();
}