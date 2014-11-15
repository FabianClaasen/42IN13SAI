#pragma once

#include <iostream>
#include <QObject>
#include <QtWidgets>
#include <QShortcut>

#include "MainWindow.h"
#include "MainController.h"
#include "TokenizerController.h"
#include "Compiler.h"
#include "VirtualMachine.h"
#include "FileIO.h"

class MainController : public QObject
{
	Q_OBJECT

	public:
		MainController();
		virtual ~MainController();

	private:
		// Variables
		MainWindow mainWindow;
		QFile currentFile;

		// Functions
		QString GetFileFromStream();
		void Setup();

	private slots:
		void Execute();
		void ClearConsole();
		void LoadFile();
		void SaveFile();
		void SaveAsFile();
};