#include "CompilerNode.h"

#include <string>

using namespace std;

string _expression;
string _identifier;
string _valueString;
CompilerNode *_valueNode;

CompilerNode::CompilerNode(string expression, string identifier, string value)
{
	_expression = expression;
	_identifier = identifier;
	_valueString = value;
}

CompilerNode::CompilerNode(string expression, string identifier, CompilerNode value)
{
	_expression = expression;
	_identifier = identifier;
	_valueNode = &value;
}

CompilerNode::CompilerNode()
{
	_expression = "";
	_identifier = "";
	_valueString = "";
	_valueNode = nullptr;
}

CompilerNode::~CompilerNode()
{
	delete _valueNode;
}
