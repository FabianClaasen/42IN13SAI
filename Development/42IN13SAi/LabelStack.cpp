#include "LabelStack.h"

LabelStack::LabelStack(std::string prefix)
{
	stack = new std::stack<std::string>();
}

LabelStack::~LabelStack()
{
	delete stack;
	stack = nullptr;
}

std::string LabelStack::Push()
{
	std::stack<std::string> _stack = *stack;
	_stack.push(prefix + std::to_string(current++));

	return _stack.top(); // Return the just added item to the stack
}

std::string LabelStack::Pop()
{
	std::stack<std::string> _stack = *stack;
	std::string temp = _stack.top(); // Save the top item to a temporary string

	_stack.pop(); // Remove the top item in the stack

	return temp;
}