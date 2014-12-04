#include "ConsoleOutput.h"

ConsoleOutput::ConsoleOutput(MainController* p_main) : main(p_main)
{

}

std::streamsize ConsoleOutput::write(const char* s, std::streamsize n)
{
	main->WriteOutput(s, n);
	return n;
}