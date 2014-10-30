//
//  ParameterException.h
//  mac_42IN13SAI
//
//  Copyright (c) 2014 42IN13SAI. All rights reserved.
//

#pragma once

#include <exception>
#include <string>

#include "ParameterExceptionType.h"

class ParameterException : public std::exception
{
private:
    std::string message;
    int amount;
    long amountGiven;
    ParameterExceptionType type;
    
    void SetErrorMessage();
public:
    const char* what() const throw();
    ParameterException(int amount, long amountGiven, ParameterExceptionType type);
    ParameterException(int amount, ParameterExceptionType type);
    ~ParameterException();
};