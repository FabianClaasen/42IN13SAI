#include "CompilerNode.h"

CompilerNode::CompilerNode(std::string p_expression, std::vector<std::shared_ptr<CompilerNode>> p_nodeParameters, std::shared_ptr<CompilerNode> p_jumpTo, bool condition) : expression(p_expression), nodeParameters(p_nodeParameters), jumpTo(p_jumpTo), condition(condition)
{
}

CompilerNode::CompilerNode(std::string p_expression, std::string p_value, bool condition) : expression(p_expression), value(p_value), condition(condition)
{
}

CompilerNode::CompilerNode(std::string p_expression, std::string p_value, std::shared_ptr<CompilerNode> p_jumpTo, bool condition) : expression(p_expression), value(p_value), jumpTo(p_jumpTo), condition(condition)
{
}

CompilerNode::CompilerNode() : expression(""), condition(condition)
{
}

CompilerNode::CompilerNode(const CompilerNode& toCopy) : expression(toCopy.expression), nodeParameters(toCopy.nodeParameters), jumpTo(toCopy.jumpTo), value(toCopy.value), condition(toCopy.condition)
{
    
}

CompilerNode::~CompilerNode()
{
	//delete jumpTo;
}

void CompilerNode::SetJumpTo(std::shared_ptr<CompilerNode> jump)
{
	jumpTo = jump;
}

std::shared_ptr<CompilerNode> CompilerNode::GetJumpTo()
{
    
    return std::shared_ptr<CompilerNode>(jumpTo.lock());
}

std::string CompilerNode::GetExpression()
{
	return expression;
}

std::string CompilerNode::GetValue()
{
	return value;
}

std::vector<std::shared_ptr<CompilerNode>> CompilerNode::GetNodeparameters()
{
	return nodeParameters;
}