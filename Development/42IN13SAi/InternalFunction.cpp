#include "InternalFunction.h"

InternalFunction::InternalFunction(Compiler* compiler) : compiler(compiler)
{
}

InternalFunction::~InternalFunction()
{
	//delete(compiler);
}

CompilerNode* InternalFunction::GetInternalFunction(MyTokenType type)
{
	switch (type)
	{
		// Default functions
	case MyTokenType::Stop:
        compiler->Match(MyTokenType::Stop);
		return getCompilerNode("$stop", 0);
	case  MyTokenType::PrintLine:
        compiler->Match(MyTokenType::PrintLine);
		return getCompilerNode("$prnt", 1);

		// Math functions
	case MyTokenType::Cosine:
		return getCompilerNode("$cos", 1);
	case MyTokenType::Sine:
		return getCompilerNode("$sin", 1);
	case MyTokenType::Tangent:
		return getCompilerNode("$tan", 1);
	case MyTokenType::Square:
		return getCompilerNode("$sqr", 1);
	case MyTokenType::Power:
		return getCompilerNode("$pow", 2);
	case MyTokenType::SquareRoot:
		return getCompilerNode("$sqrt", 1);
	case MyTokenType::CubeRoot:
		return getCompilerNode("$cbrt", 1);
	case MyTokenType::Degree:
		return getCompilerNode("$deg", 1);
	case MyTokenType::Radiant:
		return getCompilerNode("$rad", 1);
	case MyTokenType::Percent:
		return getCompilerNode("$perc", 2);
	case  MyTokenType::PerMillage:
		return getCompilerNode("$prom", 2);
	case MyTokenType::NormalLog:
		return getCompilerNode("$log", 2); // or 1
	case MyTokenType::Nlog:
		return getCompilerNode("$ln", 1);
	case MyTokenType::Modulo:
		return getCompilerNode("$mod", 2);
	default:
		break;
	}
	return new CompilerNode();
}

std::vector<CompilerNode*> InternalFunction::parseParameters(int expectedParams)
{
	// Check and parse all the parameters
	std::vector<CompilerNode*>* compiler_nodes = new std::vector<CompilerNode*>;
	compiler->Match(MyTokenType::OpenBracket);
	while (expectedParams > 0) {
        std::shared_ptr<CompilerNode> node = Parser(compiler).ParseExpression();
		compiler_nodes->push_back(node.get());
		expectedParams--;
        if (expectedParams > 0)
        {
            compiler->Match(MyTokenType::Seperator);
        }
	}
	compiler->Match(MyTokenType::CloseBracket);

	return *compiler_nodes;
}

CompilerNode* InternalFunction::getCompilerNode(std::string functionName, int params) {
	std::vector<CompilerNode*> compiler_nodes = parseParameters(params);
    
    std::vector<std::shared_ptr<CompilerNode>> sharedNodes;
    for (CompilerNode* node : compiler_nodes)
    {
        sharedNodes.push_back(std::make_shared<CompilerNode>(CompilerNode(*node)));
    }
    
    return new CompilerNode(functionName, sharedNodes, std::shared_ptr<CompilerNode>(), false);
}








