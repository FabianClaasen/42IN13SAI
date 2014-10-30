#include "CompilerNode.h"

CompilerNode::CompilerNode(std::string p_expression, std::vector<CompilerNode*> p_nodeParameters, CompilerNode *p_jumpTo) : expression(p_expression), nodeParameters(p_nodeParameters), jumpTo(p_jumpTo)
{
}

CompilerNode::CompilerNode(std::string p_expression, std::string p_value) : expression(p_expression), value(p_value)
{
}

CompilerNode::CompilerNode() : expression(""), jumpTo(nullptr)
{
}

CompilerNode::CompilerNode(const CompilerNode& toCopy) : expression(toCopy.expression), nodeParameters(toCopy.nodeParameters), jumpTo(toCopy.jumpTo), value(toCopy.value)
{
    
}

void CompilerNode::SetJumpTo(CompilerNode jump)
{
	jumpTo = &jump;
}

CompilerNode::~CompilerNode()
{
	//delete jumpTo;
}
