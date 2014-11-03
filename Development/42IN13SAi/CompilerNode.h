#pragma once

#include <string>
#include <vector>

class CompilerNode
{
private:
	std::string	expression;
	std::string value;
	std::vector<CompilerNode*> nodeParameters;
	CompilerNode *jumpTo;
	bool condition;

public:
	CompilerNode(std::string expression, std::vector<CompilerNode*> nodeParameters, CompilerNode *jumpTo, bool condition);

	//properties of compilernode
	

	CompilerNode();
    CompilerNode(const CompilerNode& toCopy);
	CompilerNode(std::string expression, std::vector<CompilerNode> nodeParameters, CompilerNode *jumpTo, bool condition);
	CompilerNode(std::string expression, std::string value, bool condition);
	~CompilerNode();

	void SetJumpTo(CompilerNode jump);
	std::string GetExpression();
	std::string GetValue();
	std::vector<CompilerNode*> GetNodeparameters();
};

