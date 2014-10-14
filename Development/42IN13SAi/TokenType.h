//
//  TokenType.h
//  part of tokenizer
//  Contains all the possible token types
//
#pragma once

enum class TokenType : int {
	// Normal tokens
	None,
    KeyIdentifier,
    Function,
    Identifier,
    If,
    Else,
    ElseIf,
    ForLoop,
    While,
    Return,
    Float,
    Special,
    EOL,
	And,
	Or,
	Seperator,
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

	// Return types
	Void,
	Float,
	None,

	// Default functions
	Stop,
	PrintLine,
	
	// Math functions
	Cosine, 
	Sine,
	Tangent,
	Square,
	Power,
	SquareRoot,
	CubeRoot,
	Degree,
	Radiant,
	Percent,
	PerMillage,
	NormalLog, 
	NormaltwoLog,
	Nlog,
	Modulo
};