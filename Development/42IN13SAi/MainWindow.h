#pragma once

#include <QMainWindow>
#include <QObject>
#include "Highlighter.h"
#include <QCompleter>
#include <qmessagebox.h>
#include <QKeyEvent>
class CodeEditor;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
		MainWindow(QWidget *parent = 0);
		virtual ~MainWindow();
		QString GetText();
		void SetText(QString text);
		QAction* GetRunAction();
		QAction* GetClearAction();
		QAction* GetLoadAction();
		QAction* GetSaveAction();
		QAction* GetSaveAsAction();
		QString OpenLoadDialog();
		QString OpenSaveDialog();


	private:
		CodeEditor* codeEditor;
		Highlighter* highlighter;
		QCompleter *completer;
		QAbstractItemModel *modelFromFile(const QString& fileName);

		QMenu *fileMenu;
		QAction *runAction;
		QAction *clearAction;
		QAction *openAction;
		QAction *saveAction;
		QAction *saveAsAction;

		void ShowMenuBar();
};