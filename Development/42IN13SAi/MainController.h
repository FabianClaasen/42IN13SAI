#pragma once

#include <boost/iostreams/concepts.hpp> 
#include <boost/iostreams/stream_buffer.hpp>
#include <boost/thread.hpp>
#include <iostream>
#include <QObject>
#include <QtWidgets>
#include <QShortcut>
#include <QMutex>
#include <QFileInfo>
#include <memory>
#include <thread>

#include "MainWindow.h"
#include "MainController.h"
#include "TokenizerController.h"
#include "Compiler.h"
#include "VirtualMachine.h"
#include "FileIO.h"
#include "ExceptionOutput.h"
#include "StopExecuteDialog.h"
#include "ErrorDialog.h"
#include "SaveException.h"
#include "LoadException.h"

class ConsoleOutput;
class MainController : public QObject
{
	Q_OBJECT

	public:
		MainController();
		virtual ~MainController();

		//void WriteOutput(const char* s, std::streamsize size);
		void WriteException(const char* output, std::streamsize size);

	private:
		// Variables
		MainWindow mainWindow;
		std::shared_ptr<TokenizerController> tokenizer_controller;
		std::shared_ptr<Compiler> compiler;
		std::shared_ptr<VirtualMachine> virtual_machine;
		std::vector<std::shared_ptr<QFile>> currentFiles;
		boost::thread* workerThread;
		StopExecuteDialog* dialog;
		QString output;
		QMutex mutex;

		double start_time;
		bool output_thread_running;

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
		void HideDialog();
		void StopVirtualMachine();

	public slots:
		void PrintException(QString exception);
		void PrintOutput(QString output);
		void VirtualMachineFinished();
};