#pragma once

#include <string>
#include <vector>

class CompilerNode
{
private:
	std::string	expression;
	std::string value;
	std::vector<CompilerNode> nodeParameters;
	CompilerNode *jumpTo;

public:
	CompilerNode(std::string expression, std::vector<CompilerNode> nodeParameters, CompilerNode *jumpTo);
	CompilerNode(std::string expression, std::string value);
	void SetJumpTo(CompilerNode jump);
	CompilerNode();
	~CompilerNode();
};

