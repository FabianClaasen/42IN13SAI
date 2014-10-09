#pragma once
#include <string>
#include <vector>

class CompilerNode
{
public:
	CompilerNode(std::string expression, std::vector<std::string> stringParameters, CompilerNode *jumpTo);
	CompilerNode(std::string expression, std::vector<CompilerNode> nodeParameters, CompilerNode *jumpTo);
	CompilerNode();
	~CompilerNode();
};

