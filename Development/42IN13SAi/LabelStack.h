#pragma once
#include <iostream>
#include <string>
#include <stack>

class LabelStack
{
private:
	std::string prefix;
	int current;
    std::stack<std::string> stack;

public:
	LabelStack(std::string prefix);
	virtual ~LabelStack();
	std::string Push();
	std::string Pop();
};

