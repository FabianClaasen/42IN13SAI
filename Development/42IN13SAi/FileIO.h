#pragma once

#include <iostream>
#include <qfile.h>
#include <qstring.h>
#include <qtextstream.h>

class FileIO
{
public:
	// Constructors
	FileIO();
	~FileIO();

	// Functions
	static QString LoadFile(QString URI);
	static void SaveFile();
};

