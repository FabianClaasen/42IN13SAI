#include "CompilerNode.h"

std::string _expression;
CompilerNode *_jumpTo;

std::vector<CompilerNode> _nodeParameters;
std::vector<std::string> _stringParameters;

CompilerNode::CompilerNode(std::string expression, std::vector<std::string> stringParameters, CompilerNode *jumpTo)
{
	_expression = expression;
	_stringParameters = stringParameters;
	_jumpTo = jumpTo;
}

CompilerNode::CompilerNode(std::string expression, std::vector<CompilerNode> nodeParameters, CompilerNode *jumpTo)
{
	_expression = expression;
	_nodeParameters = nodeParameters;
	_jumpTo = jumpTo;
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
