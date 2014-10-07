#include "InteralFunction.h"
#include "CompilerNode.h"
class DefaultFunction : public InteralFunction
{
public:
	DefaultFunction();
	virtual ~DefaultFunction();
	CompilerNode CosFunction(Token id);
	CompilerNode SinFunction(Token id);
};
