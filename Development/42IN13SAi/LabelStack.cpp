#include "LabelStack.h"

LabelStack::LabelStack(std::string prefix)
{
	stack = new std::stack<std::string>();
	LabelStack::prefix = prefix;
}

LabelStack::~LabelStack()
{
	delete stack;
	stack = nullptr;
}

std::string LabelStack::Push()
{
	stack->push(prefix + std::to_string(current++));
	return stack->top(); // Return the just added item to the stack
}

std::string LabelStack::Pop()
{
	std::string temp = stack->top(); // Save the top item to a temporary string
	stack->pop(); // Remove the top item in the stack
	return temp;
}