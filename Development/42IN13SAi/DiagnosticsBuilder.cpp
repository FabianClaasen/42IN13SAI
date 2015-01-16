//
//  DiagnosticsBuilder.cpp
//  mac_42IN13SAI
//
//  Created by Alex Nagelkerke on 08-12-14.
//  Copyright (c) 2014 42IN13SAI. All rights reserved.
//

#include "DiagnosticsBuilder.h"
#include "Token.h"

DiagnosticBuilder::DiagnosticBuilder(ExceptionEnum e)
{
    currentException = e;
    exceptionParams = new std::vector<std::string>();
}

void DiagnosticBuilder::BuildExceptionString()
{
    std::string errorString = DiagnosticBuilder::exceptionMessages.at(currentException);
    errorMessage = formatRange(errorString, *exceptionParams);
    delete exceptionParams;
}

void DiagnosticBuilder::AddString(std::string str) const
{
    exceptionParams->push_back(str);
}

std::string DiagnosticBuilder::formatRange(const std::string& format_string, const std::vector<std::string>& args)
{
    boost::format to_format(format_string);
    for(std::vector<std::string>::const_iterator it = args.begin(); it != args.end(); ++it)
    {
        to_format % *it;
    }
    return to_format.str();
}

DiagnosticBuilder::~DiagnosticBuilder()
{
    BuildExceptionString();
    std::clog << errorMessage << std::endl;
}

const std::map<ExceptionEnum, std::string> DiagnosticBuilder::exceptionMessages
{
    { ExceptionEnum::err_partner_not_found, "Partner not found for %s on line %s at position %s"},
    { ExceptionEnum::err_unkown_char, "Unrecognized character '%s' on line %s at position %s" },
    { ExceptionEnum::err_main_parameters, "Main function can't contain parameters (line %s)" },
    { ExceptionEnum::err_function_been_defined, "Redefinition of function '%s' (line %s)" },
    { ExceptionEnum::err_parameter_in_use, "Redefinition of parameter '%s' (line %s)" },
    { ExceptionEnum::err_function_in_function, "Function in function not allowed (line %s)" },
    { ExceptionEnum::err_unexpected_return, "Unexpected return type (line %s)" },
    { ExceptionEnum::err_expected_identifier, "Expected identifier (line %s)" },
    { ExceptionEnum::err_unkown_identifier, "Unkown identifier '%s' (line %s)" },
    { ExceptionEnum::err_identifier_in_use, "Redefinition of identifier '%s' (line %s)" },
    { ExceptionEnum::err_expected_keyword, "Expected keyword (line %s)" },
    { ExceptionEnum::err_zero_divide, "Can't divide by zero (line %s)" },
    { ExceptionEnum::err_var_not_found, "Variable not found '%s' (line %s)" },
    { ExceptionEnum::err_ret_as_var, "Return used as variable (line %s)" },
    { ExceptionEnum::err_unexpected_type, "Unexpected type '%s' (line %s)" },
    { ExceptionEnum::err_expected_lparen_after, "Expected left bracket after '%s' (line %s)" },
    { ExceptionEnum::err_expected_rparen_after, "Expected right bracket after '%s' (line %s)" },
    { ExceptionEnum::err_unkown_stmt, "Unkown statement '%s' (line %s)" },
    { ExceptionEnum::err_unexpected_keyword, "An UnexpectedKeywordException occured. Expected an if keyword on line %s."},
    { ExceptionEnum::err_expected_ret, "Expected return (line %s)" },
    { ExceptionEnum::err_expected_parameter, "Expected parameter (line %s at position %s)"},
    { ExceptionEnum::err_float_precision, "Float precision exception (line %s)" },
    { ExceptionEnum::err_expected_ret_type, "Expected return type (line %s)" }
};