#include "VirtualMachine.h"
#include <iostream>

VirtualMachine::VirtualMachine(SymbolTable* symboltable, SubroutineTable* subroutine, std::vector<CompilerNode> compiler_nodes)
	: mainSymboltable(symboltable), subroutineTable(subroutine), _compilernodes(compiler_nodes)
{
	function_caller = new FunctionCaller(this);
}

VirtualMachine::~VirtualMachine(){}

CompilerNode* VirtualMachine::PeekNext()
{
	CompilerNode *node = &_compilernodes.at(currentIndex + 1);
	return node;
}

CompilerNode VirtualMachine::GetNext()
{
	CompilerNode node;
    currentIndex++;
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
	// Only when there are compilernodes for gloval vars
	if (_compilernodes.size() > 0)
	{
		// First check all compilernodes for global variables
		do 
		{
			if (PeekNext() != nullptr)
			{
				CompilerNode node = VirtualMachine::GetNext();
				std::string function_call = node.get_expression();
				function_caller->Call(function_call, node);
			}
		} while (currentIndex < _compilernodes.size()-1);
	}
    // Find main subroutine
    Subroutine* mainRoutine = subroutineTable->GetSubroutine("main");
    if (mainRoutine == nullptr)
        throw std::runtime_error("No main function found");
    subSymbolTable = mainRoutine->GetSymbolTable();
    ExecuteNodes(*mainRoutine->GetCompilerNodeVector());
}

CompilerNode* VirtualMachine::ExecuteNodes(std::vector<CompilerNode> nodes)
{
    currentIndex = -1;
    _compilernodes = nodes;
    do
    {
        if (PeekNext() != nullptr)
        {
            CompilerNode node = VirtualMachine::GetNext();
            std::string function_call = node.get_expression();
            
            if (function_call == "$ret")
                return function_caller->Call(function_call, node);
            else
                function_caller->Call(function_call, node);
        }
    } while (currentIndex < _compilernodes.size()-1);
    
    return nullptr;
}

CompilerNode* VirtualMachine::CallFunction(CompilerNode node)
{
    std::string function_call = node.get_expression();
    // call the compilernode function
    return function_caller->Call(function_call, node);
}

#pragma region FunctionOperations

CompilerNode* VirtualMachine::ExecuteFunction(CompilerNode compilerNode)
{
    // Check if params is not empty
    if (compilerNode.get_nodeparamters().empty())
        throw ParameterException(1, ParameterExceptionType::NoParameters);
    
    // Get the Node parameters
    std::vector<CompilerNode *> parameters = compilerNode.get_nodeparamters();
    CompilerNode* functionNode = parameters.at(0);

    // Check if node contains the functionname
    if (functionNode->get_expression() != "$functionName")
        throw std::runtime_error("Expected function name");
    
    // Get the subroutine table and check if exists
    Subroutine* functionRoutine = subroutineTable->GetSubroutine(functionNode->get_value());
    if (functionRoutine == nullptr)
        throw std::runtime_error("Subroutine for function " + functionNode->get_value() + " not found");
    
    // Set the subSymbolTable
    subSymbolTable = functionRoutine->GetSymbolTable();
    
    // Get parameter count and check if enough parameters are given
    int parameterCount = subSymbolTable->Size();
    if (parameters.size() - 1 != parameterCount)
        throw ParameterException((int)parameters.size() - 1, parameterCount, ParameterExceptionType::IncorrectParameters);
    
    // Set the subSymbolTable symbol values
    int paramNum = 1;
    std::vector<Symbol*> vSymbols = subSymbolTable->GetSymbolVector();
    for (Symbol* symbol : vSymbols)
    {
        float param = atof(parameters.at(paramNum)->get_value().c_str());
        symbol->SetValue(param);
        paramNum++;
    }
    
    VirtualMachine::ExecuteNodes(*functionRoutine->GetCompilerNodeVector());
    return nullptr;
}

#pragma endregion FunctionOperations

#pragma region VariableOperations
CompilerNode* VirtualMachine::ExecuteAssignment(CompilerNode compilerNode)
{
	// Check if params is not empty
	if (compilerNode.get_nodeparamters().empty())
		throw ParameterException(2, ParameterExceptionType::NoParameters);
	
    // Get the Node parameters
	std::vector<CompilerNode *> parameters = compilerNode.get_nodeparamters();
    
    // Check if there aren't more than two parameters
    if (parameters.size() > 2)
        throw ParameterException(2, parameters.size(), ParameterExceptionType::IncorrectParameters);
    
	CompilerNode* param1 = parameters.at(0);
	CompilerNode* param2 = parameters.at(1);
    
	// Only go through when param is identifier
	if (param1->get_expression() == "$identifier")
	{
		// Get the value of the node -> variable
		std::string variableName = param1->get_value();
		if (param2->get_expression() != "$value")
			param2 = CallFunction(*param2);
        
        // Get the variable from symboltable
        // first check subSymbolTable
        Symbol* current_symbol = nullptr;
        if (subSymbolTable != nullptr)
            current_symbol = subSymbolTable->GetSymbol(variableName);
        
        // if not in the subSymbolTable get from main symboltable
        if (current_symbol == nullptr)
            current_symbol = mainSymboltable->GetSymbol(variableName);
        
        // Get the param value and set in temp var
        float valueToSet = atof(param2->get_value().c_str());
		current_symbol->SetValue(valueToSet);
	}
    
    return nullptr;
}

CompilerNode* VirtualMachine::ExecuteGetVariable(CompilerNode compilerNode)
{
    // Check if params is not empty
    if (compilerNode.get_value().empty())
        throw ParameterException(1, ParameterExceptionType::NoParameters);
    
    // Get the Node parameter
    std::string parameter = compilerNode.get_value();
    
    // Get the variable from symboltable
    // first check subSymbolTable
    Symbol* current_symbol = nullptr;
    if (subSymbolTable != nullptr)
        current_symbol = subSymbolTable->GetSymbol(parameter);
    
    // if not in the subSymbolTable get from main symboltable
    if (current_symbol == nullptr)
        current_symbol = mainSymboltable->GetSymbol(parameter);
    
    // Create the return node
	CompilerNode* returnNode = new CompilerNode("$value", std::to_string(current_symbol->GetValue()), false);
    
    return returnNode;
}
#pragma endregion VariableOperations

#pragma region DefaultOperations
CompilerNode* VirtualMachine::ExecutePrint(CompilerNode compilerNode)
{
	if (compilerNode.get_nodeparamters().empty())
		throw ParameterException(1, ParameterExceptionType::NoParameters);

	std::vector<CompilerNode*> parameters = compilerNode.get_nodeparamters();
	if (parameters.size() > 1)
		throw ParameterException(1, parameters.size(), ParameterExceptionType::IncorrectParameters);

	// Get first and only parameter
	CompilerNode* param1 = parameters.at(0);

	// Check if expression is not value
	if (param1->get_expression() != "$value")
		param1 = CallFunction(*param1);

	// Get the new value
	std::string valueToPrint = param1->get_value();

	// Print te new value
	std::cout << valueToPrint << std::endl;
    std::cin.get();
    
	return nullptr;
}

CompilerNode* VirtualMachine::ExecuteStop(CompilerNode compilerNode)
{
	if (compilerNode.get_nodeparamters().empty())
	{
		if (compilerNode.get_expression() == "$stop")
			std::exit(1);
		else
			throw std::runtime_error("Unknown expression type");
	}
	else
		throw new ParameterException(0, ParameterExceptionType::IncorrectParameters);
}

#pragma endregion DefaultOperations

#pragma region ConditionalStatements
CompilerNode* VirtualMachine::ExecuteLessCondition(CompilerNode compilerNode)
{
	// Check if nodeparams are not empty
	if (compilerNode.get_nodeparamters().empty())
		throw ParameterException(2, ParameterExceptionType::NoParameters);

	std::vector<CompilerNode*> parameters = compilerNode.get_nodeparamters();
	// Check if count of params is not right
	if (parameters.size() > 2)
		throw ParameterException(2, parameters.size(), ParameterExceptionType::IncorrectParameters);

	CompilerNode* param1 = parameters.at(0);
	CompilerNode* param2 = parameters.at(1);

	// Check if expression is value
	if (param1->get_expression() != "$value")
		param1 = CallFunction(*param1);
	if (param2->get_expression() != "$value")
		param2 = CallFunction(*param2);

	// Set numbers / values
	float num1 = atof(param1->get_value().c_str());
	float num2 = atof(param2->get_value().c_str());
	bool output = num1 < num2;

	// Set boolean to true if num1 < num2, else return false (inside the node)
	CompilerNode* rNode = new CompilerNode("$value", std::to_string(output), output);

	return rNode;
}

CompilerNode* VirtualMachine::ExecuteGreaterCondition(CompilerNode compilerNode)
{
	// Check if nodeparams are not empty
	if (compilerNode.get_nodeparamters().empty())
		throw ParameterException(2, ParameterExceptionType::NoParameters);

	std::vector<CompilerNode*> parameters = compilerNode.get_nodeparamters();
	// Check if count of params is not right
	if (parameters.size() > 2)
		throw ParameterException(2, parameters.size(), ParameterExceptionType::IncorrectParameters);

	CompilerNode* param1 = parameters.at(0);
	CompilerNode* param2 = parameters.at(1);

	// Check if expression is value
	if (param1->get_expression() != "$value")
		param1 = CallFunction(*param1);
	if (param2->get_expression() != "$value")
		param2 = CallFunction(*param2);

	// Set numbers / values
	float num1 = atof(param1->get_value().c_str());
	float num2 = atof(param2->get_value().c_str());
	bool output = num1 > num2;

	// Set boolean to true if num1 > num2, else return false (inside the node)
	CompilerNode* rNode = new CompilerNode("$value", std::to_string(output), output);

	return rNode;
}

CompilerNode* VirtualMachine::ExecuteEqualCondition(CompilerNode compilerNode)
{
	// Check if nodeparams are not empty
	if (compilerNode.get_nodeparamters().empty())
		throw ParameterException(2, ParameterExceptionType::NoParameters);

	std::vector<CompilerNode*> parameters = compilerNode.get_nodeparamters();
	// Check if count of params is not right
	if (parameters.size() > 2)
		throw ParameterException(2, parameters.size(), ParameterExceptionType::IncorrectParameters);

	CompilerNode* param1 = parameters.at(0);
	CompilerNode* param2 = parameters.at(1);

	// Check if expression is value
	if (param1->get_expression() != "$value")
		param1 = CallFunction(*param1);
	if (param2->get_expression() != "$value")
		param2 = CallFunction(*param2);

	// Set numbers / values
	float num1 = atof(param1->get_value().c_str());
	float num2 = atof(param2->get_value().c_str());
	bool output = num1 == num2;

	// Set boolean to true if num1 == num2, else return false (inside the node)
	CompilerNode* rNode = new CompilerNode("$value", std::to_string(output), output);

	return rNode;
}

CompilerNode* VirtualMachine::ExecuteNotEqualCondition(CompilerNode compilerNode)
{
	// Check if nodeparams are not empty
	if (compilerNode.get_nodeparamters().empty())
		throw ParameterException(2, ParameterExceptionType::NoParameters);

	std::vector<CompilerNode*> parameters = compilerNode.get_nodeparamters();
	// Check if count of params is not right
	if (parameters.size() > 2)
		throw ParameterException(2, parameters.size(), ParameterExceptionType::IncorrectParameters);

	CompilerNode* param1 = parameters.at(0);
	CompilerNode* param2 = parameters.at(1);

	// Check if expression is value
	if (param1->get_expression() != "$value")
		param1 = CallFunction(*param1);
	if (param2->get_expression() != "$value")
		param2 = CallFunction(*param2);

	// Set numbers / values
	float num1 = atof(param1->get_value().c_str());
	float num2 = atof(param2->get_value().c_str());
	bool output = num1 != num2;

	// Set boolean to true if num1 != num2, else return false (inside the node)
	CompilerNode* rNode = new CompilerNode("$value", std::to_string(output), output);

	return rNode;
}
#pragma endregion ConditionalStatements

#pragma region SimpleMath
CompilerNode *VirtualMachine::ExecuteAddOperation(CompilerNode compilerNode)
{
    if (compilerNode.get_nodeparamters().empty())
        throw ParameterException(2, ParameterExceptionType::NoParameters);
    
    // Get the Node parameters
    std::vector<CompilerNode *> parameters = compilerNode.get_nodeparamters();
    
    // Check if there aren't more than two parameters
    if (parameters.size() > 2)
        throw ParameterException(2, parameters.size(), ParameterExceptionType::IncorrectParameters);
    
    CompilerNode* param1 = parameters.at(0);
    CompilerNode* param2 = parameters.at(1);
    
    // Check if the parameters are a value or another function call
    // if function call, execute function
    if (param1->get_expression() != "$value")
        param1 = CallFunction(*param1);
    if (param2->get_expression() != "$value")
        param2 = CallFunction(*param2);
    
    // Parse the parameters to a float for mathmatic operation
    float num1 = atof(param1->get_value().c_str());
    float num2 = atof(param2->get_value().c_str());
    float output = num1 + num2;
    
    // Create a new value compilernode to return
	CompilerNode* rNode = new CompilerNode("$value", std::to_string(output), false);
     
	return rNode;
}

CompilerNode* VirtualMachine::ExecuteMinusOperation(CompilerNode compilerNode)
{
    if (compilerNode.get_nodeparamters().empty())
        throw ParameterException(2, ParameterExceptionType::NoParameters);
    
    // Get the Node parameters
    std::vector<CompilerNode *> parameters = compilerNode.get_nodeparamters();
    
    // Check if there aren't more than two parameters
    if (parameters.size() > 2)
        throw ParameterException(2, parameters.size(), ParameterExceptionType::IncorrectParameters);
    
    CompilerNode* param1 = parameters.at(0);
    CompilerNode* param2 = parameters.at(1);
    
    // Check if the parameters are a value or another function call
    // if function call, execute function
    if (param1->get_expression() != "$value")
        param1 = CallFunction(*param1);
    if (param2->get_expression() != "$value")
        param2 = CallFunction(*param2);
    
    // Parse the parameters to a float for mathmatic operation
    float num1 = atof(param1->get_value().c_str());
    float num2 = atof(param2->get_value().c_str());
	float output = num1 - num2;
    
    // Create a new value compilernode to return
	CompilerNode* rNode = new CompilerNode("$value", std::to_string(output),false);
    
    return rNode;
}

CompilerNode* VirtualMachine::ExecuteMultiplyOperation(CompilerNode compilerNode)
{
    if (compilerNode.get_nodeparamters().empty())
        throw ParameterException(2, ParameterExceptionType::NoParameters);
    
    // Get the Node parameters
    std::vector<CompilerNode *> parameters = compilerNode.get_nodeparamters();
    
    // Check if there aren't more than two parameters
    if (parameters.size() > 2)
        throw ParameterException(2, parameters.size(), ParameterExceptionType::IncorrectParameters);
    
    CompilerNode* param1 = parameters.at(0);
    CompilerNode* param2 = parameters.at(1);
    
    // Check if the parameters are a value or another function call
    // if function call, execute function
    if (param1->get_expression() != "$value")
        param1 = CallFunction(*param1);
    if (param2->get_expression() != "$value")
        param2 = CallFunction(*param2);
    
    // Parse the parameters to a float for mathmatic operation
    float num1 = atof(param1->get_value().c_str());
    float num2 = atof(param2->get_value().c_str());
	float output = num1 * num2;
    
    // Create a new value compilernode to return
	CompilerNode* rNode = new CompilerNode("$value", std::to_string(output),false);
    
    return rNode;
}

CompilerNode* VirtualMachine::ExecuteDivideOperation(CompilerNode compilerNode)
{
    if (compilerNode.get_nodeparamters().empty())
        throw ParameterException(2, ParameterExceptionType::NoParameters);
    
    // Get the Node parameters
    std::vector<CompilerNode *> parameters = compilerNode.get_nodeparamters();
    
    // Check if there aren't more than two parameters
    if (parameters.size() > 2)
        throw ParameterException(2, parameters.size(), ParameterExceptionType::IncorrectParameters);
    
    CompilerNode* param1 = parameters.at(0);
    CompilerNode* param2 = parameters.at(1);
    
    // Check if the parameters are a value or another function call
    // if function call, execute function
    if (param1->get_expression() != "$value")
        param1 = CallFunction(*param1);
    if (param2->get_expression() != "$value")
        param2 = CallFunction(*param2);
    
    // Parse the parameters to a float for mathmatic operation
    float num1 = atof(param1->get_value().c_str());
    float num2 = atof(param2->get_value().c_str());
	float output = num1 / num2;
    
    // Create a new value compilernode to return
	CompilerNode* rNode = new CompilerNode("$value", std::to_string(output),false);
    
    return rNode;
}

CompilerNode* VirtualMachine::ExecuteModuloOperation(CompilerNode compilerNode)
{
	if (compilerNode.get_nodeparamters().empty())
		throw ParameterException(2, ParameterExceptionType::NoParameters);

	// Get the Node parameters
	std::vector<CompilerNode *> parameters = compilerNode.get_nodeparamters();

	// Check if there aren't more than two parameters
	if (parameters.size() > 2)
		throw ParameterException(2, parameters.size(), ParameterExceptionType::IncorrectParameters);

	CompilerNode* param1 = parameters.at(0);
	CompilerNode* param2 = parameters.at(1);

	// Check if the parameters are a value or another function call
	// if function call, execute function
	if (param1->get_expression() != "$value")
		param1 = CallFunction(*param1);
	if (param2->get_expression() != "$value")
		param2 = CallFunction(*param2);

	// Parse the parameters to a float for mathmatic operation
	float num1 = atof(param1->get_value().c_str());
	float num2 = atof(param2->get_value().c_str());
	float output = fmod(num1, num2);

	// Create a new value compilernode to return
	CompilerNode* rNode = new CompilerNode("$value", std::to_string(output),false);

	return rNode;
}

#pragma endregion SimpleMath

#pragma region ComplexMath

CompilerNode* VirtualMachine::ExecuteSinOperation(CompilerNode compilerNode)
{
    if (compilerNode.get_nodeparamters().empty())
        throw ParameterException(1, ParameterExceptionType::NoParameters);
    
    // Get the Node parameters
    std::vector<CompilerNode *> parameters = compilerNode.get_nodeparamters();
    
    // Check if there aren't more than one parameter
    if (parameters.size() > 1)
        throw ParameterException(1, parameters.size(), ParameterExceptionType::IncorrectParameters);
    
    CompilerNode* param1 = parameters.at(0);
    
    // Check if the parameters are a value or another function call
    // if function call, execute function
    if (param1->get_expression() != "$value")
        param1 = CallFunction(*param1);
    
    // Parse the parameters to a float for mathmatic operation
    float num1 = atof(param1->get_value().c_str());
    float output = std::sin(num1);
    
    // Create a new value compilernode to return
	CompilerNode* rNode = new CompilerNode("$value", std::to_string(output),false);
    
    return rNode;
}

CompilerNode* VirtualMachine::ExecuteCosOperation(CompilerNode compilerNode)
{
    if (compilerNode.get_nodeparamters().empty())
        throw ParameterException(1, ParameterExceptionType::NoParameters);
    
    // Get the Node parameters
    std::vector<CompilerNode *> parameters = compilerNode.get_nodeparamters();
    
    // Check if there aren't more than one parameter
    if (parameters.size() > 1)
        throw ParameterException(1, parameters.size(), ParameterExceptionType::IncorrectParameters);
    
    CompilerNode* param1 = parameters.at(0);
    
    // Check if the parameters are a value or another function call
    // if function call, execute function
    if (param1->get_expression() != "$value")
        param1 = CallFunction(*param1);
    
    // Parse the parameters to a float for mathmatic operation
    float num1 = atof(param1->get_value().c_str());
	float output = std::cos(num1);
    
    // Create a new value compilernode to return
	CompilerNode* rNode = new CompilerNode("$value", std::to_string(output),false);
    
    return rNode;
}

CompilerNode* VirtualMachine::ExecuteTanOperation(CompilerNode compilerNode)
{
    if (compilerNode.get_nodeparamters().empty())
        throw ParameterException(1, ParameterExceptionType::NoParameters);
    
    // Get the Node parameters
    std::vector<CompilerNode *> parameters = compilerNode.get_nodeparamters();
    
    // Check if there aren't more than one parameter
    if (parameters.size() > 1)
        throw ParameterException(1, parameters.size(), ParameterExceptionType::IncorrectParameters);
    
    CompilerNode* param1 = parameters.at(0);
    
    // Check if the parameters are a value or another function call
    // if function call, execute function
    if (param1->get_expression() != "$value")
        param1 = CallFunction(*param1);
    
    // Parse the parameters to a float for mathmatic operation
    float num1 = atof(param1->get_value().c_str());
	float output = std::tan(num1);
    
    // Create a new value compilernode to return
	CompilerNode* rNode = new CompilerNode("$value", std::to_string(output),false);
    
    return rNode;
}

#pragma endregion ComplexMath




