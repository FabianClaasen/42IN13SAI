#include "InteralFunction.h"
#include "DefaultFunction.h"
#include <string>

InteralFunction::InteralFunction()
{
	DefaultFunction* _default = new DefaultFunction();
}


InteralFunction::~InteralFunction()
{
}

void InteralFunction::GetInternalFunction()
{
	std::string identifier("if");
	switch (identifier)
	{
	case "if":
		_default->ParseIfStatement();
			break;
	defaut:break;
	}
}