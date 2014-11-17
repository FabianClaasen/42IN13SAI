#pragma once

#include <QMainWindow>
#include <QObject>
#include "Highlighter.h"
#include <QCompleter>
#include "CodeCompleter.h"
#include <qmessagebox.h>
#include <QKeyEvent>
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
		QCompleter *completer;
		CodeCompleter *completingTextEdit;
		QAbstractItemModel *modelFromFile(const QString& fileName);
		QAction *runAction;
		QAction *clearAction;

		void showMenuBar();

//protected:
//	void keyPressEvent(QKeyEvent* e);
};