//
//  TokenType.h
//  part of tokenizer
//  Contains all the possible token types
//
#pragma once

enum TokenType {
    KeyIdentifier,
    Function,
    Identifier,
    If,
    Else,
    ElseIf,
    ForLoop,
    While,
    Return,
    Integer,
    Double,
    Boolean,
    Special,
    EOL,
    OperatorPlus,
    OperatorMinus,
    OperatorDivide,
    OperatorRaised,
    OperatorMultiply,
    UniOperatorPlus,
    UniOperatorMinus,
    GreaterThan,
    LowerThan,
    GreaterOrEqThan,
    LowerOrEqThan,
    Equals,
    OpenMethod,
    CloseMethod,
    OpenCurlyBracket,
    CloseCurlyBracket,
    OpenBracket,
    CloseBracket,
    Comparator,
	Cosine,
	Sine,
	Tangent
};