#include "CompilerNode.h"

std::string _expression;
CompilerNode *_jumpTo;

std::string _value;

std::vector<CompilerNode> _nodeParameters;

CompilerNode::CompilerNode(std::string expression, std::vector<CompilerNode> nodeParameters, CompilerNode *jumpTo)
{
	_expression = expression;
	_nodeParameters = nodeParameters;
	_jumpTo = jumpTo;
}

CompilerNode::CompilerNode(std::string value)
{
	_value = value;
}

CompilerNode::CompilerNode()
{
	_expression = "";
	_jumpTo = nullptr;
}

CompilerNode::~CompilerNode()
{
	delete _jumpTo;
}
