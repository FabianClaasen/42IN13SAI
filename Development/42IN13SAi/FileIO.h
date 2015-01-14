#pragma once

#include <iostream>
#include <qfile.h>
#include <qstring.h>
#include <qtextstream.h>
#include <memory>

#include "LoadException.h"
#include "SaveException.h"

class FileIO
{
public:
	// Constructors
	FileIO();
	~FileIO();

	// Functions
	static QString LoadFile(QString URI);
	static void SaveFile(QString URI, QString code);
	static void SaveFile(std::shared_ptr<QFile> file, QString code);
};

