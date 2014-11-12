#pragma once

#include <QMainWindow>
#include <QObject>
#include "Highlighter.h"
class CodeEditor;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
		MainWindow(QWidget *parent = 0);
		~MainWindow();
		QString getText();

	private slots:
		void execute();
		void clearConsole();

	private:
		CodeEditor* codeEditor;
		Highlighter* highlighter;
		QString getFileFromStream();
		void showMenuBar();
};