#include "CompilerNode.h"

<<<<<<< HEAD
std::string _expression;
CompilerNode *_jumpTo;

std::vector<CompilerNode> *_nodeParameters;
std::vector<std::string> *_stringParameters;
=======
using namespace std;
>>>>>>> 24acf3a57308c73cc10a380cd43c8be74401a676

CompilerNode::CompilerNode(std::string expression, std::vector<std::string> *stringParameters, CompilerNode *jumpTo)
{
	_expression = expression;
	_stringParameters = stringParameters;
	_nodeParameters = nullptr;
	_jumpTo = jumpTo;
}

CompilerNode::CompilerNode(std::string expression, std::vector<CompilerNode> *nodeParameters, CompilerNode *jumpTo)
{
	_expression = expression;
	_nodeParameters = nodeParameters;
	_stringParameters = nullptr;
	_jumpTo = jumpTo;
}

CompilerNode::CompilerNode()
{
	_expression = "";
	_jumpTo = nullptr;
	_nodeParameters = nullptr;
	_stringParameters = nullptr;
}

CompilerNode::~CompilerNode()
{
	delete _jumpTo, _nodeParameters, _stringParameters;
}
