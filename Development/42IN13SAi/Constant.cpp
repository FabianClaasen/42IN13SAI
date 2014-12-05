#include "Constant.h"


Constant::Constant(Compiler* compiler) : compiler(compiler)
{
}


Constant::~Constant()
{
}


std::shared_ptr<CompilerNode> Constant::getCompilerNode(std::string functionName) {
	std::vector<std::shared_ptr<CompilerNode>> compiler_nodes;
	return std::make_shared<CompilerNode>(functionName, compiler_nodes, nullptr, false);
}

std::shared_ptr<CompilerNode> Constant::GetConstant(MyTokenType type)
{
	switch (type)
	{
		// Default functions
	case MyTokenType::Math_E:
		//compiler->Match(MyTokenType::Math_E);
		return getCompilerNode("$mathe");
	case  MyTokenType::Math_PI:
		//compiler->Match(MyTokenType::Math_PI);
		return getCompilerNode("$mathpi");	
	default:
		break;
	}
	return nullptr;
}
