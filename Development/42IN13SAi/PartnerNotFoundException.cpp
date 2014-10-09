//
//  ParseException.cpp
//  tokenizer
//
#include "PartnerNotFoundException.h"

PartnerNotFoundException::PartnerNotFoundException(std::string m) : message(m)
{}

PartnerNotFoundException::~PartnerNotFoundException()
{}

const char* PartnerNotFoundException::what() const throw()
{
    return message.c_str();
}