//
//  ParseException.cpp
//  tokenizer
//
#include <stdio.h>
#include "PartnerNotFoundException.h"

PartnerNotFoundException::PartnerNotFoundException(std::string m) : message(m)
{}


const char* PartnerNotFoundException::what() const throw()
{
    return message.c_str();
}