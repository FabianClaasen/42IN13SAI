#pragma once

#include <boost/iostreams/concepts.hpp> 
#include <boost/iostreams/stream_buffer.hpp>
#include <iostream>
#include <QObject>
#include <QtWidgets>
#include <QShortcut>
#include <QFileInfo>
#include <memory>

#include "MainWindow.h"
#include "MainController.h"
#include "TokenizerController.h"
#include "Compiler.h"
#include "VirtualMachine.h"
#include "FileIO.h"
#include "ConsoleOutput.h"

class ConsoleOutput;
class MainController : public QObject
{
	Q_OBJECT

	public:
		MainController();
		virtual ~MainController();

		void WriteOutput(const char* s, std::streamsize size);

	private:
		// Variables
		MainWindow mainWindow;
		std::vector<std::shared_ptr<QFile>> currentFiles;
		QString output;

		// Functions
		std::string GetFileFromStream();
		void Setup();

	private slots:
		void Execute();
		void ClearConsole();
		void NewFile();
		void LoadFile();
		void SaveFile();
		void SaveAsFile();
		void Quit();
		void CloseTab(int index);
};