#include "InteralFunction.h"
#include "DefaultFunction.h"
#include "InternalFunctionType.h"
#include <string>

InteralFunction::InteralFunction(DefaultFunction &_default)
{
	_default.GetInternalFunction;
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
