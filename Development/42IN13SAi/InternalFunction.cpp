#include "InternalFunction.h"

InternalFunction::InternalFunction(Compiler* compiler) : compiler(compiler)
{
}

InternalFunction::~InternalFunction()
{
	//delete(compiler);
}

std::shared_ptr<CompilerNode> InternalFunction::GetInternalFunction(MyTokenType type)
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
	case MyTokenType::CosineR:
		return getCompilerNode("$cosr", 1);
	case MyTokenType::SineR:
		return getCompilerNode("$sinr", 1);
	case MyTokenType::TangentR:
		return getCompilerNode("$tanr", 1);
	case MyTokenType::Square:
		return getCompilerNode("$sqr", 1);
	case MyTokenType::Cube:
		return getCompilerNode("$cbc", 1);
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
	case  MyTokenType::Permillage:
		return getCompilerNode("$perm", 2);
	case MyTokenType::CommonLog:
		return getCompilerNode("$log10", 1); 
	case MyTokenType::BinaryLog:
		return getCompilerNode("$log2", 1);
	case MyTokenType::NaturalLog:
		return getCompilerNode("$ln", 1);
	case MyTokenType::Log:
		return getCompilerNode("$log", 2);
	case MyTokenType::Modulo:
		return getCompilerNode("$mod", 2);
	case MyTokenType::Gcd:
		return getCompilerNode("$gcd", 2);
	case MyTokenType::Discriminant:
		return getCompilerNode("$dis", 3);
	case MyTokenType::AbcFormula:
		compiler->Match(MyTokenType::AbcFormula);
		return getCompilerNode("$abc", 3);
	case MyTokenType::Fibonacci:
		return getCompilerNode("$fib", 1);
	case MyTokenType::Pythagoras:
		return getCompilerNode("$pyt", 2); 
	case MyTokenType::PythagorasH:
		return getCompilerNode("$pyth", 2); 
	case MyTokenType::Distance:
		return getCompilerNode("$dist", 2);
	case MyTokenType::Velocity:
		return getCompilerNode("$velo", 2);
	case MyTokenType::Time:
		return getCompilerNode("$time", 2);
	case MyTokenType::AverageVelocity:
		return getCompilerNode("$avgv", 2);
	case MyTokenType::Acceleration:
		return getCompilerNode("$accl", 2);
	default:
		break;
	}
	return nullptr;
}

std::vector<std::shared_ptr<CompilerNode>> InternalFunction::parseParameters(int expectedParams)
{
	// Check and parse all the parameters
	std::vector<std::shared_ptr<CompilerNode>> compiler_nodes;
	compiler->Match(MyTokenType::OpenBracket);
	while (expectedParams > 0) {
		std::shared_ptr<CompilerNode> node(Parser(compiler).ParseExpression());
		
        if (node != nullptr)
        {
            compiler_nodes.push_back(node);
            expectedParams--;
            if (expectedParams > 0)
            {
                compiler->Match(MyTokenType::Separator);
            }
        }
        else
        {
            compiler->Diag(ExceptionEnum::err_expected_parameter) << compiler->GetCurrent().LineNumber << compiler->GetCurrent().LinePosition;
            
            compiler->SkipUntil(MyTokenType::CloseBracket);
            break;
        }
	}
	compiler->Match(MyTokenType::CloseBracket);

	return compiler_nodes;
}

std::shared_ptr<CompilerNode> InternalFunction::getCompilerNode(std::string functionName, int params) {
	std::vector<std::shared_ptr<CompilerNode>> compiler_nodes = parseParameters(params);
	return std::make_shared<CompilerNode>(functionName, compiler_nodes, nullptr, false);
}