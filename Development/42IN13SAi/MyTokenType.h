//
//  MyTokenType.h
//  part of tokenizer
//  Contains all the possible token types
//
#pragma once

enum class MyTokenType : int {
	// Normal tokens
    KeyIdentifier,
    Var,
	Array,
    MainFunction,
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
	Separator,
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
    FloatReturn,
	None,

	// Default functions
	Stop,
	PrintLine,
	
	// Math functions
	Cosine, 
	Sine,
	Tangent,
	Square,
	Cube,
	Power,
	SquareRoot,
	CubeRoot,
	Degree,
	Radiant,
	Percent,
	Permillage,
	CommonLog, 
	BinaryLog,
	NaturalLog,
	Log,
	Modulo,
	Gcd,
	Discriminant,
	AbcFormula
};