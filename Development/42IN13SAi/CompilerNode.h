#pragma once
#include <string>
#include <vector>

class CompilerNode
{
public:
	CompilerNode(std::string expression, std::vector<CompilerNode> nodeParameters, CompilerNode *jumpTo);
	CompilerNode(std::string value);
	CompilerNode();
	~CompilerNode();
};

