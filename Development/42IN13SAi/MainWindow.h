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
		virtual ~MainWindow();
		QString getText();
		QAction *getRunAction();
		QAction *getClearAction();

	private:
		CodeEditor* codeEditor;
		Highlighter* highlighter;
		QAction *runAction;
		QAction *clearAction;

		void showMenuBar();
};