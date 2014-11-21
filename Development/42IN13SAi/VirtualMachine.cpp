#include "VirtualMachine.h"
#include <iostream>

VirtualMachine::VirtualMachine(SymbolTable* symboltable, SubroutineTable* subroutine, std::shared_ptr<LinkedList> globalsNodes)
: globalsSymboltable(symboltable), subroutineTable(subroutine), globalsList(globalsNodes)
{
	function_caller = std::unique_ptr<FunctionCaller>(new FunctionCaller(this));
	
	currentSymbolTable = nullptr;
	currentSubroutine = nullptr;
}

VirtualMachine::VirtualMachine(const VirtualMachine &other) : globalsSymboltable(other.globalsSymboltable), currentSubroutine(other.currentSubroutine), currentSymbolTable(other.currentSymbolTable), subroutineTable(other.subroutineTable), globalsList(other.globalsList)
{
	function_caller = std::unique_ptr<FunctionCaller>(new FunctionCaller(this));
}

VirtualMachine& VirtualMachine::operator=(const VirtualMachine& other)
{
	if (this != &other)
	{
		VirtualMachine* cVirtualMachine = new VirtualMachine(other);
		return *cVirtualMachine;
		
	}
	return *this;
}

VirtualMachine::~VirtualMachine(){}

std::shared_ptr<CompilerNode> VirtualMachine::GetNext(std::shared_ptr<LinkedList> nodes)
{
	std::shared_ptr<ListNode> node = nodes->GetNext();

	if (!node)
	{
		node.reset();
		throw MissingCompilerNodeException("Compilernode missing");
	}

	return node->GetData();
}


void VirtualMachine::ExecuteCode()
{
	// Only when there are compilernodes for global vars
	if (globalsList->size() > 0)
	{
		// First check all compilernodes for global variables
		std::shared_ptr<CompilerNode> node;
		do
		{
			node = VirtualMachine::GetNext(std::shared_ptr<LinkedList>(globalsList));
			std::string function_call = node->GetExpression();
			function_caller->Call(function_call, *node);

		} while (node != globalsList->GetTailData());
	}
	
	// Find main subroutine
	currentSubroutine = subroutineTable->GetSubroutine("main");
	
	if (currentSubroutine == nullptr)
		throw MissingMainFunctionException("No main function found");
	
	currentSymbolTable = currentSubroutine->GetSymbolTable();
	
	// Get the main nodes and execute them
	ExecuteNodes(std::shared_ptr<LinkedList>(currentSubroutine->GetCompilerNodeCollection()));
}

std::shared_ptr<CompilerNode> VirtualMachine::ExecuteNodes(std::shared_ptr<LinkedList> nodes)
{
	// Get the subroutine name
	std::string subroutineName = currentSubroutine->name;
	// Insert the linkedlist for this subroutine
	nodeLists.insert(std::pair<std::string, std::shared_ptr<LinkedList>>
						 (subroutineName, nodes));

	if (nodes->size() > 0)
	{
		std::shared_ptr<CompilerNode> node;
		do
		{
			node = VirtualMachine::GetNext(nodeLists[subroutineName]);

			if (node)
			{
				// Get the function name
				std::string function_call = node->GetExpression();

				// if it is a return function return the CompilerNode
				if (function_call == "$ret")
				{
					return function_caller->Call(function_call, *node);
				}
				else if (function_call == "$doNothing")
				{
					// Set the current node to the partner of the donothing node
					nodeLists[subroutineName]->SetCurrent(node->GetJumpTo());
					node = nodeLists[subroutineName]->GetCurrentData();
				}
				else
					function_caller->Call(function_call, *node);
			}

		} while (node != std::shared_ptr<CompilerNode>());
	}
	
	// Erase the LinkedList
	nodeLists.erase(subroutineName);
	
	return nullptr;
}

std::shared_ptr<CompilerNode> VirtualMachine::CallFunction(CompilerNode node)
{
	std::string function_call = node.GetExpression();
	// call the compilernode function
	return function_caller->Call(function_call, node);
}

#pragma region FunctionOperations

std::shared_ptr<CompilerNode> VirtualMachine::ExecuteFunction(CompilerNode compilerNode)
{
	// Check if params is not empty
	if (compilerNode.GetNodeparameters().empty())
		throw ParameterException(1, ParameterExceptionType::NoParameters);
	
	// Get the Node parameters
	std::vector<std::shared_ptr<CompilerNode>> parameters = compilerNode.GetNodeparameters();
	std::shared_ptr<CompilerNode> functionNode = parameters.at(0);

	// Check if node contains the functionname
	if (functionNode->GetExpression() != "$functionName")
		throw FunctionNameExpectedException("Expected function name");
	
	// Get the subroutine table and check if exists
	currentSubroutine = subroutineTable->GetSubroutine(functionNode->GetValue());
	if (currentSubroutine == nullptr)
		throw SubroutineNotFoundException("Subroutine for function " + functionNode->GetValue() + " not found");
	
	// Set the currentSymbolTable
	currentSymbolTable = currentSubroutine->GetSymbolTable();
	
	// Get parameter count and check if enough parameters are given
	int parameterCount = currentSymbolTable->ParameterSize();
	if (parameters.size() - 1 != parameterCount)
		throw ParameterException((int)parameters.size() - 1, parameterCount, ParameterExceptionType::IncorrectParameters);
	
	// Set the currentSymbolTable symbol values
	int paramNum = 1;
	std::vector<Symbol*> vSymbols = currentSymbolTable->GetSymbolVector();
	for (Symbol* symbol : vSymbols)
	{
		std::shared_ptr<CompilerNode> param = parameters.at(paramNum);
		if (param->GetExpression() != "$value")
			param = CallFunction(*param);
		
		float fParam = atof(param->GetValue().c_str());
		symbol->SetValue(fParam);
		paramNum++;
	}
	
	return VirtualMachine::ExecuteNodes(currentSubroutine->GetCompilerNodeCollection());
}
std::shared_ptr<CompilerNode> VirtualMachine::ExecuteReturn(CompilerNode compilerNode)
{
	// Check if params is not empty
	if (compilerNode.GetNodeparameters().empty())
		throw ParameterException(1, ParameterExceptionType::NoParameters);
	
	// Get the Node parameters
	std::vector<std::shared_ptr<CompilerNode>> parameters = compilerNode.GetNodeparameters();
	
	// Check if there aren't more than two parameters
	if (parameters.size() > 1)
		throw ParameterException(1, parameters.size(), ParameterExceptionType::IncorrectParameters);
	
	std::shared_ptr<CompilerNode> param1 = parameters.at(0);
	if (param1->GetExpression() != "$value")
		param1 = CallFunction(*param1);
	
	// Create the return node
	std::shared_ptr<CompilerNode> returnNode = std::make_shared<CompilerNode>(CompilerNode("$value", param1->GetValue(), false));
	
	return returnNode;
}

#pragma endregion FunctionOperations

#pragma region VariableOperations
std::shared_ptr<CompilerNode> VirtualMachine::ExecuteAssignment(CompilerNode compilerNode)
{
	// Check if params is not empty
	if (compilerNode.GetNodeparameters().empty())
		throw ParameterException(2, ParameterExceptionType::NoParameters);
	
	// Get the Node parameters
	std::vector<std::shared_ptr<CompilerNode>> parameters = compilerNode.GetNodeparameters();
	
	// Check if there aren't more than two parameters
	if (parameters.size() > 2)
		throw ParameterException(2, parameters.size(), ParameterExceptionType::IncorrectParameters);
	
	std::shared_ptr<CompilerNode> param1 = parameters.at(0);
	std::shared_ptr<CompilerNode> param2 = parameters.at(1);
	
	// Only go through when param is identifier
	if (param1->GetExpression() == "$identifier")
	{
		// Get the value of the node -> variable
		std::string variableName = param1->GetValue();
		if (param2->GetExpression() != "$value")
			param2 = CallFunction(*param2);
		
		// Get the variable from symboltable
		// first check subSymbolTable
		Symbol* current_symbol = nullptr;
		if (currentSymbolTable != nullptr)
			current_symbol = currentSymbolTable->GetSymbol(variableName);
		
		// if not in the subSymbolTable get from globals symboltable
		if (current_symbol == nullptr)
			current_symbol = globalsSymboltable->GetSymbol(variableName);
		
		// Get the param value and set in temp var
		float valueToSet = atof(param2->GetValue().c_str());
		current_symbol->SetValue(valueToSet);
	}
	
	return nullptr;
}

std::shared_ptr<CompilerNode> VirtualMachine::ExecuteGetVariable(CompilerNode compilerNode)
{
	// Check if params is not empty
	if (compilerNode.GetValue().empty())
		throw ParameterException(1, ParameterExceptionType::NoParameters);
	
	// Get the Node parameter
	std::string parameter = compilerNode.GetValue();
	
	// Get the variable from symboltable
	// first check subSymbolTable
	Symbol* current_symbol = nullptr;
	if (currentSymbolTable != nullptr)
		current_symbol = currentSymbolTable->GetSymbol(parameter);
	
	// if not in the subSymbolTable get from globals symboltable
	if (current_symbol == nullptr)
		current_symbol = globalsSymboltable->GetSymbol(parameter);
	
	// Create the return node
	std::shared_ptr<CompilerNode> returnNode = std::make_shared<CompilerNode>(CompilerNode("$value", std::to_string(current_symbol->GetValue()), false));
	
	return returnNode;
}
#pragma endregion VariableOperations

#pragma region DefaultOperations
std::shared_ptr<CompilerNode> VirtualMachine::ExecutePrint(CompilerNode compilerNode)
{
	if (compilerNode.GetNodeparameters().empty())
		throw ParameterException(1, ParameterExceptionType::NoParameters);

	std::vector<std::shared_ptr<CompilerNode>> parameters = compilerNode.GetNodeparameters();
	if (parameters.size() > 1)
		throw ParameterException(1, parameters.size(), ParameterExceptionType::IncorrectParameters);

	// Get first and only parameter
	std::shared_ptr<CompilerNode> param1 = parameters.at(0);

	// Check if expression is not value
	if (param1->GetExpression() != "$value")
		param1 = CallFunction(*param1);

	// Get the new value
	std::string valueToPrint = param1->GetValue();

	// Print te new value
	std::cout << valueToPrint << std::endl;
	//std::cin.get();
	
	return nullptr;
}

std::shared_ptr<CompilerNode> VirtualMachine::ExecuteStop(CompilerNode compilerNode)
{
	if (compilerNode.GetNodeparameters().empty())
	{
		if (compilerNode.GetExpression() == "$stop")
			std::exit(1);
		else
			throw UnknownExpressionException("Unknown expression type");
	}
	else
		throw new ParameterException(0, ParameterExceptionType::IncorrectParameters);
}

#pragma endregion DefaultOperations

#pragma region LoopOperations

std::shared_ptr<CompilerNode> VirtualMachine::ExecuteWhile(CompilerNode compilerNode)
{
	// Check if nodeparams are not empty
	if (compilerNode.GetNodeparameters().empty())
		throw ParameterException(1, ParameterExceptionType::NoParameters);
	
	std::vector<std::shared_ptr<CompilerNode>> parameters = compilerNode.GetNodeparameters();
	// Check if count of params is not right
	if (parameters.size() > 1)
		throw ParameterException(1, parameters.size(), ParameterExceptionType::IncorrectParameters);
	
	std::shared_ptr<CompilerNode> condition = parameters.at(0);
	
	if (condition->GetExpression() != "$value")
		condition = CallFunction(*condition);
	
	// Check if condition is true.
	if (condition->GetValue() == "1")
	{
		return nullptr;
	}
	else
	{
		// Condition is false, move linkedlist to donothing node
		nodeLists[currentSubroutine->name]->SetCurrent(compilerNode.GetJumpTo(), true);
		return nullptr;
	}
}

std::shared_ptr<CompilerNode> VirtualMachine::ExecuteFor(CompilerNode compilerNode)
{
	// Check if nodeparams are not empty
	if (compilerNode.GetNodeparameters().empty())
		throw ParameterException(3, ParameterExceptionType::NoParameters);

	std::vector<std::shared_ptr<CompilerNode>> parameters = compilerNode.GetNodeparameters();
	// Check if count of params is not right
	if (parameters.size() > 3)
		throw ParameterException(3, parameters.size(), ParameterExceptionType::IncorrectParameters);

	std::shared_ptr<CompilerNode> assignment = parameters.at(0);
	if (assignment->GetExpression() == "$assignment")
	{
		std::shared_ptr<CompilerNode> identifier = assignment->GetNodeparameters().at(0);
		// If the symboltable doesn't contain the variable it's the first time in the for loop.
		// Add the symbol to the table so it doesn't keep getting reset
		if (currentSymbolTable->GetSymbol(identifier->GetValue()) == nullptr)
		{
			currentSymbolTable->AddSymbol(Symbol(identifier->GetValue(), MyTokenType::Float, SymbolKind::Local));
			CallFunction(*assignment);
		}
	}

	std::shared_ptr<CompilerNode> condition = parameters.at(1);
	if (condition->GetExpression() != "$value")
		condition = CallFunction(*condition);

	// Check if condition is true.
	if (condition->GetValue() == "1")
	{
		std::shared_ptr<CompilerNode> expression = parameters.at(2);
		if (expression->GetExpression() != "$value")
			expression = CallFunction(*expression);

		return nullptr;
	}
	else
	{
		// Condition is false, move linkedlist to donothing node
		nodeLists[currentSubroutine->name]->SetCurrent(compilerNode.GetJumpTo(), true);
		return nullptr;
	}
}

#pragma endregion LoopOperations

#pragma region ConditionalStatements
std::shared_ptr<CompilerNode> VirtualMachine::ExecuteLessCondition(CompilerNode compilerNode)
{
	// Check if nodeparams are not empty
	if (compilerNode.GetNodeparameters().empty())
		throw ParameterException(2, ParameterExceptionType::NoParameters);

	std::vector<std::shared_ptr<CompilerNode>> parameters = compilerNode.GetNodeparameters();
	// Check if count of params is not right
	if (parameters.size() > 2)
		throw ParameterException(2, parameters.size(), ParameterExceptionType::IncorrectParameters);

	std::shared_ptr<CompilerNode> param1 = parameters.at(0);
	std::shared_ptr<CompilerNode> param2 = parameters.at(1);

	// Check if expression is value
	if (param1->GetExpression() != "$value")
		param1 = CallFunction(*param1);
	if (param2->GetExpression() != "$value")
		param2 = CallFunction(*param2);

	// Set numbers / values
	float num1 = atof(param1->GetValue().c_str());
	float num2 = atof(param2->GetValue().c_str());
	bool output = num1 < num2;

	// Set boolean to true if num1 < num2, else return false (inside the node)
	return std::make_shared<CompilerNode>(CompilerNode("$value", std::to_string(output), false));
}

std::shared_ptr<CompilerNode> VirtualMachine::ExecuteLessOrEqCondition(CompilerNode compilerNode)
{
	// Check if nodeparams are not empty
	if (compilerNode.GetNodeparameters().empty())
		throw ParameterException(2, ParameterExceptionType::NoParameters);

	std::vector<std::shared_ptr<CompilerNode>> parameters = compilerNode.GetNodeparameters();
	// Check if count of params is not right
	if (parameters.size() > 2)
		throw ParameterException(2, parameters.size(), ParameterExceptionType::IncorrectParameters);

	std::shared_ptr<CompilerNode> param1 = parameters.at(0);
	std::shared_ptr<CompilerNode> param2 = parameters.at(1);

	// Check if expression is value
	if (param1->GetExpression() != "$value")
		param1 = CallFunction(*param1);
	if (param2->GetExpression() != "$value")
		param2 = CallFunction(*param2);

	// Set numbers / values
	float num1 = atof(param1->GetValue().c_str());
	float num2 = atof(param2->GetValue().c_str());
	bool output = num1 <= num2;

	// Set boolean to true if num1 < num2, else return false (inside the node)
	return std::make_shared<CompilerNode>(CompilerNode("$value", std::to_string(output), false));
}

std::shared_ptr<CompilerNode> VirtualMachine::ExecuteGreaterCondition(CompilerNode compilerNode)
{
	// Check if nodeparams are not empty
	if (compilerNode.GetNodeparameters().empty())
		throw ParameterException(2, ParameterExceptionType::NoParameters);

	std::vector<std::shared_ptr<CompilerNode>> parameters = compilerNode.GetNodeparameters();
	// Check if count of params is not right
	if (parameters.size() > 2)
		throw ParameterException(2, parameters.size(), ParameterExceptionType::IncorrectParameters);

	std::shared_ptr<CompilerNode> param1 = parameters.at(0);
	std::shared_ptr<CompilerNode> param2 = parameters.at(1);

	// Check if expression is value
	if (param1->GetExpression() != "$value")
		param1 = CallFunction(*param1);
	if (param2->GetExpression() != "$value")
		param2 = CallFunction(*param2);

	// Set numbers / values
	float num1 = atof(param1->GetValue().c_str());
	float num2 = atof(param2->GetValue().c_str());
	bool output = num1 > num2;

	// Set boolean to true if num1 > num2, else return false (inside the node)
	return std::make_shared<CompilerNode>(CompilerNode("$value", std::to_string(output), false));
}

std::shared_ptr<CompilerNode> VirtualMachine::ExecuteGreaterOrEqCondition(CompilerNode compilerNode)
{
	// Check if nodeparams are not empty
	if (compilerNode.GetNodeparameters().empty())
		throw ParameterException(2, ParameterExceptionType::NoParameters);

	std::vector<std::shared_ptr<CompilerNode>> parameters = compilerNode.GetNodeparameters();
	// Check if count of params is not right
	if (parameters.size() > 2)
		throw ParameterException(2, parameters.size(), ParameterExceptionType::IncorrectParameters);

	std::shared_ptr<CompilerNode> param1 = parameters.at(0);
	std::shared_ptr<CompilerNode> param2 = parameters.at(1);

	// Check if expression is value
	if (param1->GetExpression() != "$value")
		param1 = CallFunction(*param1);
	if (param2->GetExpression() != "$value")
		param2 = CallFunction(*param2);

	// Set numbers / values
	float num1 = atof(param1->GetValue().c_str());
	float num2 = atof(param2->GetValue().c_str());
	bool output = num1 >= num2;

	// Set boolean to true if num1 > num2, else return false (inside the node)
	return std::make_shared<CompilerNode>(CompilerNode("$value", std::to_string(output), false));
}

std::shared_ptr<CompilerNode> VirtualMachine::ExecuteEqualCondition(CompilerNode compilerNode)
{
	// Check if nodeparams are not empty
	if (compilerNode.GetNodeparameters().empty())
		throw ParameterException(2, ParameterExceptionType::NoParameters);

	std::vector<std::shared_ptr<CompilerNode>> parameters = compilerNode.GetNodeparameters();
	// Check if count of params is not right
	if (parameters.size() > 2)
		throw ParameterException(2, parameters.size(), ParameterExceptionType::IncorrectParameters);

	std::shared_ptr<CompilerNode> param1 = parameters.at(0);
	std::shared_ptr<CompilerNode> param2 = parameters.at(1);

	// Check if expression is value
	if (param1->GetExpression() != "$value")
		param1 = CallFunction(*param1);
	if (param2->GetExpression() != "$value")
		param2 = CallFunction(*param2);

	// Set numbers / values
	float num1 = atof(param1->GetValue().c_str());
	float num2 = atof(param2->GetValue().c_str());
	bool output = num1 == num2;

	// Set boolean to true if num1 == num2, else return false (inside the node)
	return std::make_shared<CompilerNode>(CompilerNode("$value", std::to_string(output), false));
}

std::shared_ptr<CompilerNode> VirtualMachine::ExecuteNotEqualCondition(CompilerNode compilerNode)
{
	// Check if nodeparams are not empty
	if (compilerNode.GetNodeparameters().empty())
		throw ParameterException(2, ParameterExceptionType::NoParameters);

	std::vector<std::shared_ptr<CompilerNode>> parameters = compilerNode.GetNodeparameters();
	// Check if count of params is not right
	if (parameters.size() > 2)
		throw ParameterException(2, parameters.size(), ParameterExceptionType::IncorrectParameters);

	std::shared_ptr<CompilerNode> param1 = parameters.at(0);
	std::shared_ptr<CompilerNode> param2 = parameters.at(1);

	// Check if expression is value
	if (param1->GetExpression() != "$value")
		param1 = CallFunction(*param1);
	if (param2->GetExpression() != "$value")
		param2 = CallFunction(*param2);

	// Set numbers / values
	float num1 = atof(param1->GetValue().c_str());
	float num2 = atof(param2->GetValue().c_str());
	bool output = num1 != num2;

	// Set boolean to true if num1 != num2, else return false (inside the node)
	return std::make_shared<CompilerNode>(CompilerNode("$value", std::to_string(output), false));
}
#pragma endregion ConditionalStatements

#pragma region SimpleMath
std::shared_ptr<CompilerNode> VirtualMachine::ExecuteAddOperation(CompilerNode compilerNode)
{
	if (compilerNode.GetNodeparameters().empty())
		throw ParameterException(2, ParameterExceptionType::NoParameters);
	
	// Get the Node parameters
	std::vector<std::shared_ptr<CompilerNode>> parameters = compilerNode.GetNodeparameters();
	
	// Check if there aren't more than two parameters
	if (parameters.size() > 2)
		throw ParameterException(2, parameters.size(), ParameterExceptionType::IncorrectParameters);
	
	std::shared_ptr<CompilerNode> param1 = parameters.at(0);
	std::shared_ptr<CompilerNode> param2 = parameters.at(1);
	
	// Check if the parameters are a value or another function call
	// if function call, execute function
	if (param1->GetExpression() != "$value")
		param1 = CallFunction(*param1);
	if (param2->GetExpression() != "$value")
		param2 = CallFunction(*param2);
	
	// Parse the parameters to a float for mathmatic operation
	float num1 = atof(param1->GetValue().c_str());
	float num2 = atof(param2->GetValue().c_str());
	float output = num1 + num2;
	
	// Create a new value compilernode to return
	return std::make_shared<CompilerNode>(CompilerNode("$value", std::to_string(output), false));
}

std::shared_ptr<CompilerNode> VirtualMachine::ExecuteMinusOperation(CompilerNode compilerNode)
{
	if (compilerNode.GetNodeparameters().empty())
		throw ParameterException(2, ParameterExceptionType::NoParameters);
	
	// Get the Node parameters
	std::vector<std::shared_ptr<CompilerNode>> parameters = compilerNode.GetNodeparameters();
	
	// Check if there aren't more than two parameters
	if (parameters.size() > 2)
		throw ParameterException(2, parameters.size(), ParameterExceptionType::IncorrectParameters);
	
	std::shared_ptr<CompilerNode> param1 = parameters.at(0);
	std::shared_ptr<CompilerNode> param2 = parameters.at(1);
	
	// Check if the parameters are a value or another function call
	// if function call, execute function
	if (param1->GetExpression() != "$value")
		param1 = CallFunction(*param1);
	if (param2->GetExpression() != "$value")
		param2 = CallFunction(*param2);
	
	// Parse the parameters to a float for mathmatic operation
	float num1 = atof(param1->GetValue().c_str());
	float num2 = atof(param2->GetValue().c_str());
	float output = num1 - num2;
	
	// Create a new value compilernode to return
	return std::make_shared<CompilerNode>(CompilerNode("$value", std::to_string(output), false));
}

std::shared_ptr<CompilerNode> VirtualMachine::ExecuteMultiplyOperation(CompilerNode compilerNode)
{
	if (compilerNode.GetNodeparameters().empty())
		throw ParameterException(2, ParameterExceptionType::NoParameters);
	
	// Get the Node parameters
	std::vector<std::shared_ptr<CompilerNode>> parameters = compilerNode.GetNodeparameters();
	
	// Check if there aren't more than two parameters
	if (parameters.size() > 2)
		throw ParameterException(2, parameters.size(), ParameterExceptionType::IncorrectParameters);
	
	std::shared_ptr<CompilerNode> param1 = parameters.at(0);
	std::shared_ptr<CompilerNode> param2 = parameters.at(1);
	
	// Check if the parameters are a value or another function call
	// if function call, execute function
	if (param1->GetExpression() != "$value")
		param1 = CallFunction(*param1);
	if (param2->GetExpression() != "$value")
		param2 = CallFunction(*param2);
	
	// Parse the parameters to a float for mathmatic operation
	float num1 = atof(param1->GetValue().c_str());
	float num2 = atof(param2->GetValue().c_str());
	float output = num1 * num2;
	
	// Create a new value compilernode to return
	return std::make_shared<CompilerNode>(CompilerNode("$value", std::to_string(output), false));
}

std::shared_ptr<CompilerNode> VirtualMachine::ExecuteDivideOperation(CompilerNode compilerNode)
{
	if (compilerNode.GetNodeparameters().empty())
		throw ParameterException(2, ParameterExceptionType::NoParameters);
	
	// Get the Node parameters
	std::vector<std::shared_ptr<CompilerNode>> parameters = compilerNode.GetNodeparameters();
	
	// Check if there aren't more than two parameters
	if (parameters.size() > 2)
		throw ParameterException(2, parameters.size(), ParameterExceptionType::IncorrectParameters);
	
	std::shared_ptr<CompilerNode> param1 = parameters.at(0);
	std::shared_ptr<CompilerNode> param2 = parameters.at(1);
	
	// Check if the parameters are a value or another function call
	// if function call, execute function
	if (param1->GetExpression() != "$value")
		param1 = CallFunction(*param1);
	if (param2->GetExpression() != "$value")
		param2 = CallFunction(*param2);
	
	// Parse the parameters to a float for mathmatic operation
	float num1 = atof(param1->GetValue().c_str());
	float num2 = atof(param2->GetValue().c_str());
	float output = num1 / num2;
	
	// Create a new value compilernode to return
	return std::make_shared<CompilerNode>(CompilerNode("$value", std::to_string(output), false));
}

std::shared_ptr<CompilerNode> VirtualMachine::ExecuteModuloOperation(CompilerNode compilerNode)
{
	if (compilerNode.GetNodeparameters().empty())
		throw ParameterException(2, ParameterExceptionType::NoParameters);

	// Get the Node parameters
	std::vector<std::shared_ptr<CompilerNode>> parameters = compilerNode.GetNodeparameters();

	// Check if there aren't more than two parameters
	if (parameters.size() > 2)
		throw ParameterException(2, parameters.size(), ParameterExceptionType::IncorrectParameters);

	std::shared_ptr<CompilerNode> param1 = parameters.at(0);
	std::shared_ptr<CompilerNode> param2 = parameters.at(1);

	// Check if the parameters are a value or another function call
	// if function call, execute function
	if (param1->GetExpression() != "$value")
		param1 = CallFunction(*param1);
	if (param2->GetExpression() != "$value")
		param2 = CallFunction(*param2);

	// Parse the parameters to a float for mathmatic operation
	float num1 = atof(param1->GetValue().c_str());
	float num2 = atof(param2->GetValue().c_str());
	float output = fmod(num1, num2);

	// Create a new value compilernode to return
	return std::make_shared<CompilerNode>(CompilerNode("$value", std::to_string(output), false));
}

std::shared_ptr<CompilerNode> VirtualMachine::ExecuteUniMinOperation(CompilerNode compilerNode)
{
	if (compilerNode.GetNodeparameters().empty())
		throw ParameterException(1, ParameterExceptionType::NoParameters);
	
	// Get the Node parameters
	std::vector<std::shared_ptr<CompilerNode>> parameters = compilerNode.GetNodeparameters();
	
	// Check if there aren't more than two parameters
	if (parameters.size() > 1)
		throw ParameterException(1, parameters.size(), ParameterExceptionType::IncorrectParameters);
	
	std::shared_ptr<CompilerNode> param1 = parameters.at(0);
	
	// Check if the parameters are a value or another function call
	// if function call, execute function
	if (param1->GetExpression() != "$value")
		param1 = CallFunction(*param1);
	
	// Parse the parameters to a float for mathmatic operation
	float num1 = atof(param1->GetValue().c_str());
	float output = num1 - 1;
	
	// Create a new value compilernode to return
	return std::make_shared<CompilerNode>(CompilerNode("$value", std::to_string(output), false));
}

std::shared_ptr<CompilerNode> VirtualMachine::ExecuteUniPlusOperation(CompilerNode compilerNode)
{
	if (compilerNode.GetNodeparameters().empty())
		throw ParameterException(1, ParameterExceptionType::NoParameters);
	
	// Get the Node parameters
	std::vector<std::shared_ptr<CompilerNode>> parameters = compilerNode.GetNodeparameters();
	
	// Check if there aren't more than two parameters
	if (parameters.size() > 1)
		throw ParameterException(1, parameters.size(), ParameterExceptionType::IncorrectParameters);
	
	std::shared_ptr<CompilerNode> param1 = parameters.at(0);
	std::string identifierName = param1->GetValue();
	
	// Check if the parameters are a value or another function call
	// if function call, execute function
	if (param1->GetExpression() != "$value")
		param1 = CallFunction(*param1);
	
	// Parse the parameters to a float for mathmatic operation
	float num1 = atof(param1->GetValue().c_str());
	float output = num1 + 1;
	
	// Create a new value compilernode to return
	return std::make_shared<CompilerNode>(CompilerNode("$value", std::to_string(output), false));
}

#pragma endregion SimpleMath

#pragma region ComplexMath

std::shared_ptr<CompilerNode> VirtualMachine::ExecuteSinOperation(CompilerNode compilerNode)
{
	if (compilerNode.GetNodeparameters().empty())
		throw ParameterException(1, ParameterExceptionType::NoParameters);
	
	// Get the Node parameters
	std::vector<std::shared_ptr<CompilerNode>> parameters = compilerNode.GetNodeparameters();
	
	// Check if there aren't more than one parameter
	if (parameters.size() > 1)
		throw ParameterException(1, parameters.size(), ParameterExceptionType::IncorrectParameters);
	
	std::shared_ptr<CompilerNode> param1 = parameters.at(0);
	
	// Check if the parameters are a value or another function call
	// if function call, execute function
	if (param1->GetExpression() != "$value")
		param1 = CallFunction(*param1);
	
	// Parse the parameters to a float for mathmatic operation
	float num1 = atof(param1->GetValue().c_str());
	float output = std::sin(num1);
	
	// Create a new value compilernode to return
	return std::make_shared<CompilerNode>(CompilerNode("$value", std::to_string(output), false));
}

std::shared_ptr<CompilerNode> VirtualMachine::ExecuteCosOperation(CompilerNode compilerNode)
{
	if (compilerNode.GetNodeparameters().empty())
		throw ParameterException(1, ParameterExceptionType::NoParameters);
	
	// Get the Node parameters
	std::vector<std::shared_ptr<CompilerNode>> parameters = compilerNode.GetNodeparameters();
	
	// Check if there aren't more than one parameter
	if (parameters.size() > 1)
		throw ParameterException(1, parameters.size(), ParameterExceptionType::IncorrectParameters);
	
	std::shared_ptr<CompilerNode> param1 = parameters.at(0);
	
	// Check if the parameters are a value or another function call
	// if function call, execute function
	if (param1->GetExpression() != "$value")
		param1 = CallFunction(*param1);
	
	// Parse the parameters to a float for mathmatic operation
	float num1 = atof(param1->GetValue().c_str());
	float output = std::cos(num1);
	
	// Create a new value compilernode to return
	return std::make_shared<CompilerNode>(CompilerNode("$value", std::to_string(output), false));
}

std::shared_ptr<CompilerNode> VirtualMachine::ExecuteTanOperation(CompilerNode compilerNode)
{
	if (compilerNode.GetNodeparameters().empty())
		throw ParameterException(1, ParameterExceptionType::NoParameters);
	
	// Get the Node parameters
	std::vector<std::shared_ptr<CompilerNode>> parameters = compilerNode.GetNodeparameters();
	
	// Check if there aren't more than one parameter
	if (parameters.size() > 1)
		throw ParameterException(1, parameters.size(), ParameterExceptionType::IncorrectParameters);
	
	std::shared_ptr<CompilerNode> param1 = parameters.at(0);
	
	// Check if the parameters are a value or another function call
	// if function call, execute function
	if (param1->GetExpression() != "$value")
		param1 = CallFunction(*param1);
	
	// Parse the parameters to a float for mathmatic operation
	float num1 = atof(param1->GetValue().c_str());
	float output = std::tan(num1);
	
	// Create a new value compilernode to return
	return std::make_shared<CompilerNode>(CompilerNode("$value", std::to_string(output), false));
}

#pragma endregion ComplexMath




