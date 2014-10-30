#include "VirtualMachine.h"

VirtualMachine::VirtualMachine(SymbolTable symboltable, SubroutineTable subroutine, std::vector<CompilerNode> compiler_nodes)
	: _symboltable(symboltable), _subroutine(subroutine), _compilernodes(compiler_nodes)
{
	function_caller = new FunctionCaller(this);
}

VirtualMachine::~VirtualMachine(){}

CompilerNode* VirtualMachine::PeekNext()
{
    int peekIndex = (currentIndex != 0) ? currentIndex + 1 : 0;
	CompilerNode *node = &_compilernodes.at(peekIndex);
	return node;
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
				function_caller->Call(function_call, node);
			}
		}
		// after all compilernodes pushed in array do all subroutines

		// Find main subroutine
		_subroutine.GetSubroutine("main");
	}
}

CompilerNode* VirtualMachine::CallFunction(CompilerNode node)
{
    std::string function_call = node.get_expression();
    // call the compilernode function
    return function_caller->Call(function_call, node);
}

CompilerNode VirtualMachine::ExecuteAssignment(CompilerNode compilerNode)
{
	return compilerNode;
}

CompilerNode *VirtualMachine::ExecuteAddOperation(CompilerNode compilerNode)
{
    if (compilerNode.get_nodeparamters().empty())
        throw std::runtime_error("Function expects 2 parameters");
    
    std::vector<CompilerNode *> parameters = compilerNode.get_nodeparamters();
    CompilerNode* param1 = parameters.at(0);
    CompilerNode* param2 = parameters.at(1);
    
    if (param1->get_expression() != "$value")
        param1 = CallFunction(*param1);
    if (param2->get_expression() != "$value")
        param2 = CallFunction(*param2);
    
    float num1 = atof(param1->get_value().c_str());
    float num2 = atof(param2->get_value().c_str());
    float addition = num1 + num2;
    
    CompilerNode* rNode = new CompilerNode("$value", std::to_string(addition));
     
	return rNode;
}

CompilerNode VirtualMachine::ExecuteMinusOperation(CompilerNode compilerNode)
{
	return compilerNode;
}


