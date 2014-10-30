#include "VirtualMachine.h"

VirtualMachine::VirtualMachine(SymbolTable* symboltable, SubroutineTable* subroutine, std::vector<CompilerNode> compiler_nodes)
	: _symboltable(symboltable), _subroutine(subroutine), _compilernodes(compiler_nodes)
{
	function_caller = new FunctionCaller(this);
}

VirtualMachine::~VirtualMachine(){}

CompilerNode* VirtualMachine::PeekNext()
{
	if (currentIndex != 0)
		return &_compilernodes.at(currentIndex + 1);
	else
		return &_compilernodes.at(currentIndex);
}

CompilerNode VirtualMachine::GetNext()
{
	if (currentIndex != 0)
		currentIndex++;

	CompilerNode node;
	if (currentIndex <= _compilernodes.size()-1 && _compilernodes.size() > 0)
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
	// Only when there are compilernodes
	if (_compilernodes.size() > 0)
	{
		// First check all compilernodes for global variables
		while (currentIndex <= _compilernodes.size())
		{
			if (PeekNext() != nullptr)
			{
				// TODO : function name & params from compilernode
				// Check for actions / functions
				// get main subroutine as first
				CompilerNode node = VirtualMachine::GetNext();
				std::string function_call = node.get_expression();
				/*std::vector<CompilerNode*> params;
				std::string value;
				if (node.get_nodeparamters().size() > 0)
					params = node.get_nodeparamters();
				else 
					std::string value = node.get_value();*/

				// push received node in array
				_received_compilernodes.push_back(function_caller->Call(function_call, node));
			}
		}
		// after all compilernodes pushed in array do all subroutines

		// Find main subroutine
		_subroutine->GetSubroutine("main");
	}
}

CompilerNode VirtualMachine::ExecuteAssignment(CompilerNode compilerNode)
{
	// Check if params is not empty
	if (compilerNode.get_nodeparamters().empty())
		throw std::runtime_error("Function needs at least 1 parameter");
	
	// Get the value of the node -> variable
	std::string new_node = compilerNode.get_nodeparamters().at(0)->get_value();
	Symbol* symbol = _symboltable->GetSymbol(new_node);
	symbol->SetValue(1);
	return compilerNode;
}

CompilerNode VirtualMachine::ExecuteAddOperation(CompilerNode compilerNode)
{
	return compilerNode;
}

CompilerNode VirtualMachine::ExecuteMinusOperation(CompilerNode compilerNode)
{
	return compilerNode;
}


