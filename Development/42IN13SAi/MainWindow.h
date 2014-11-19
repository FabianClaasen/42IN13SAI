#pragma once

#include <QMainWindow>
#include <QObject>
#include <QCompleter>
#include <QKeyEvent>
#include <QTabWidget>
#include <QFileInfo>
#include <memory>
#include <vector>

#include "Highlighter.h"

class CodeEditor;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
		MainWindow(QWidget *parent = 0);
		virtual ~MainWindow();
		int GetCurrentTabPosition();
		QString GetText();
		void AddFile(QFileInfo* info, QString text);
		QAction* GetRunAction();
		QAction* GetClearAction();
		QAction* GetLoadAction();
		QAction* GetSaveAction();
		QAction* GetSaveAsAction();
		QString OpenLoadDialog();
		QString OpenSaveDialog();

	private:
		int position;
		std::vector<CodeEditor*> codeEditorVector;
		Highlighter* highlighter;
		QCompleter* completer;
		QTabWidget* tabs;

		QAbstractItemModel *modelFromFile(const QString& fileName);
		CodeEditor* CreateEditor();

		// Menu include all menu items
		QMenu *fileMenu;
		QAction *runAction, *clearAction, *openAction, *saveAction, *saveAsAction;

		void ShowMenuBar();
};