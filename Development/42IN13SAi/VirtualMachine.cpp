#include "VirtualMachine.h"

VirtualMachine::VirtualMachine(SymbolTable symboltable, SubroutineTable subroutine, std::vector<CompilerNode> compiler_nodes)
	: _symboltable(symboltable), _subroutine(subroutine), _compilernodes(compiler_nodes)
{
	function_caller = new FunctionCaller(this);
}


VirtualMachine::~VirtualMachine(){}

CompilerNode* VirtualMachine::PeekNext()
{
	CompilerNode node = _compilernodes.at(currentIndex + 1);
	return &node;
}

CompilerNode VirtualMachine::GetNext()
{
	currentIndex++;
	CompilerNode node;
	if (currentIndex != _compilernodes.size() - 1)
	{
		node = _compilernodes.at(currentIndex);
	}
	else
	{
		throw std::runtime_error("Compilernode missing");
	}

	return node;
}

void VirtualMachine::ExecuteCode()
{
	// First check all compilernodes for global variables
	//while (currentIndex <= _compilernodes.size() - 1)
	//{
	//	CompilerNode node = VirtualMachine::GetNext();
	//	std::string function_call = node.get_expression();

	//	// push received node in array
	//	received_compilernodes.push_back(function_caller->Call(function_call, node));
	//}

	// Find main subroutine
	//_subroutine.
}


