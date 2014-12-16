//
//  DiagnosticsBuilder.h
//  mac_42IN13SAI
//
//  Created by Alex Nagelkerke on 08-12-14.
//  Copyright (c) 2014 42IN13SAI. All rights reserved.
//

#ifndef __mac_42IN13SAI__DiagnosticsBuilder__
#define __mac_42IN13SAI__DiagnosticsBuilder__

#include <iostream>
#include <vector>
#include <map>
#include <boost/format.hpp>

#include "ExceptionEnum.h"
//#include "ExceptionMessages.h"

class Token;
class DiagnosticBuilder {
public:
    DiagnosticBuilder(const Token& tok, ExceptionEnum e);
    ~DiagnosticBuilder();
    
    
    
    void BuildExceptionString();
    void AddString(std::string str) const;
    
private:
    //Token token;
    static const std::map<ExceptionEnum, std::string> exceptionMessages;
    ExceptionEnum currentException;
    std::string errorMessage;
    std::vector<std::string>* exceptionParams;
    
    std::string formatRange(const std::string& format_string, const std::vector<std::string>& args);
};

inline const DiagnosticBuilder &operator<<(const DiagnosticBuilder &DB, const std::string str)
{
    DB.AddString(str);
    return DB;
}

inline const DiagnosticBuilder &operator<<(const DiagnosticBuilder &DB, const int int_value)
{
    DB.AddString(std::to_string(int_value));
    return DB;
}

#endif /* defined(__mac_42IN13SAI__DiagnosticsBuilder__) */
