#pragma once

#include <boost/iostreams/concepts.hpp> 
#include <boost/iostreams/stream_buffer.hpp>
#include <iostream>

#include "MainController.h"

namespace bio = boost::iostreams;

class MainController;
class ExceptionOutput : public bio::sink
{
private:
	MainController* main;

public:
	ExceptionOutput(MainController* p_main);
	std::streamsize write(const char* s, std::streamsize n);
};

