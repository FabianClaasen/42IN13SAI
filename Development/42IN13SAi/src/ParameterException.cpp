//
//  ParameterException.cpp
//  mac_42IN13SAI
//
//  Created by Alex Nagelkerke on 30-10-14.
//  Copyright (c) 2014 42IN13SAI. All rights reserved.
//

#include "ParameterException.h"

ParameterException::ParameterException(int a, long ag, ParameterExceptionType t) : amount(a), amountGiven(ag), type(t)
{
    SetErrorMessage();
}

ParameterException::ParameterException(int a, ParameterExceptionType t) : amount(a), type(t)
{
    SetErrorMessage();
}

ParameterException::~ParameterException()
{}

void ParameterException::SetErrorMessage()
{
    switch (type) {
        case IncorrectParameters:
            message = "Expected " + std::to_string(amount) + " parameters but " + std::to_string(amountGiven) + " parameters were given";
            break;
        case NoParameters:
            message = "Function expects at least " + std::to_string(amount) + " parameters but none given";
        default:
            message = "";
            break;
    }
}

const char* ParameterException::what() const throw()
{
    return message.c_str();
}