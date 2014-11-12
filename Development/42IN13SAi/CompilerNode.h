#pragma once

#include <string>
#include <vector>
#include <memory>

class CompilerNode
{
private:
	std::string	expression;
	std::string value;
	std::vector<std::shared_ptr<CompilerNode>> nodeParameters;
	std::weak_ptr<CompilerNode> jumpTo;
	bool condition;

public:
	CompilerNode(std::string expression, std::vector<std::shared_ptr<CompilerNode>> nodeParameters, std::shared_ptr<CompilerNode> jumpTo, bool condition);

	//properties of compilernode
	

	CompilerNode();
    CompilerNode(const CompilerNode& toCopy);
	CompilerNode(std::string expression, std::vector<CompilerNode> nodeParameters, std::shared_ptr<CompilerNode> jumpTo, bool condition);
    CompilerNode(std::string expression, std::string value, std::shared_ptr<CompilerNode> jumpTo, bool condition);
	CompilerNode(std::string expression, std::string value, bool condition);
	~CompilerNode();

	void SetJumpTo(std::shared_ptr<CompilerNode> jump);
    std::shared_ptr<CompilerNode> GetJumpTo();
	std::string GetExpression();
	std::string GetValue();
	std::vector<std::shared_ptr<CompilerNode>> GetNodeparameters();
};

