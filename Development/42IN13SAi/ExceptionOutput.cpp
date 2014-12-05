#include "ExceptionOutput.h"

ExceptionOutput::ExceptionOutput(MainController* p_main) : main(p_main)
{

}

std::streamsize ExceptionOutput::write(const char* s, std::streamsize n)
{
	main->WriteException(s, n);
	return n;
}