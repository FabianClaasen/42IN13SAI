#include "InteralFunction.h"
class DefaultFunction : public InteralFunction
{
public:
	DefaultFunction();
	virtual ~DefaultFunction();
	void ParseIfStateMent();
	void ParseWhileStateMent();
};
