#pragma once
#include <string>
#ifndef IINTERALFUNCTION_H
#define IINTERALFUNCTION_H
class IInteralFunction
{
public:
	virtual ~IInteralFunction(){}
	virtual void GetInternalFunction(std::string) = 0;
};

#endif