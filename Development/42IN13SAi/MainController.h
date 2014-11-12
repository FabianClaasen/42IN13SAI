#pragma once

#include <QObject>

#include "MainWindow.h"

class MainController : public QObject
{
	Q_OBJECT

	public:
		MainController();
		virtual ~MainController();

	private:
		MainWindow mainWindow;
		QString getFileFromStream();
		void setup();

	private slots:
		void execute();
		void clearConsole();
};