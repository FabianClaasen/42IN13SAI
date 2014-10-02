#pragma once
#include <string>

using namespace std;

class CompilerNode
{
public:
	CompilerNode(string expression, string identifier, string value);
	~CompilerNode();
};

