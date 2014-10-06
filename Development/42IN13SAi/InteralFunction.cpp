#include "InteralFunction.h"
#include "InternalFunctionType.h"
#include <string>

InteralFunction::InteralFunction()
{
	_default = new DefaultFunction();
	GetInternalFunction("gddsdgsfsd");
}


InteralFunction::~InteralFunction()
{
	
}

void InteralFunction::GetInternalFunction(std::string identifier)
{
	/*switch (identifier)
	{
	case InternalFunctionType::IF:
		_default->ParseIfStatement();
			break;
	defaut:break;
	}*/
}