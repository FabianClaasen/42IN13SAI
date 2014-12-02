//
//  ParseException.cpp
//  tokenizer
//
#include "ParseException.h"

ParseException::ParseException(std::string m) : message(m)
{}

ParseException::~ParseException()
{}

const char* ParseException::what() const throw()
{
    return message.c_str();
}