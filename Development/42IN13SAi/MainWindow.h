#pragma once

#include <QMainWindow>
#include <QObject>
#include "Highlighter.h"
#include <QCompleter>
#include <QKeyEvent>
#include <memory>

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
		QAction* GetQuitAction();
		QString OpenLoadDialog();
		QString OpenSaveDialog();

	private:
		CodeEditor* codeEditor;
		Highlighter* highlighter;
		QCompleter* completer;

		QAbstractItemModel *modelFromFile(const QString& fileName);

		// Menu include all menu items
		QMenu *fileMenu;
		QAction *runAction, *clearAction, *openAction, *saveAction, *saveAsAction, *quitAction;

		void ShowMenuBar();
};