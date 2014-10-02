#include "CompilerNode.h"

#include <string>

using namespace std;

string _expression;
string _identifier;
string _value;

CompilerNode::CompilerNode(string expression, string identifier, string value)
{
	_expression = expression;
	_identifier = identifier;
	_value = value;
}


CompilerNode::~CompilerNode()
{
}
