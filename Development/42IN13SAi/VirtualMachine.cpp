#include "VirtualMachine.h"


VirtualMachine::VirtualMachine(SymbolTable symboltable, SubroutineTable subroutine, std::list<CompilerNode> compiler_nodes)
	: _symboltable(symboltable), _subroutine(subroutine), _compilernodes(compiler_nodes){}


VirtualMachine::~VirtualMachine(){}

void VirtualMachine::ExecuteCode()
{

}


