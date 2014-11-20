#pragma once

#include <QMainWindow>
#include <QObject>
#include <QCompleter>
#include <QKeyEvent>
#include <QTabWidget>
#include <QFileInfo>
#include <memory>
#include <vector>
#include <QtWidgets>
#include <QMenu>
#include <QKeyEvent>

#include "CodeEditor.h"
#include "TokenizerController.h"
#include "Compiler.h"
#include "VirtualMachine.h"
#include "Highlighter.h"
#include "ExceptionWindow.h"

class CodeEditor;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
		MainWindow(QWidget *parent = 0);
		virtual ~MainWindow();
		int GetCurrentTabPosition();
		void RemoveTab(int index);
		void AddNewTab();
		void addException(std::string exception);
		void AddFile(QFileInfo* info, QString text);
		void SetTabTitle(QFileInfo* info);
		QString GetText();
		QAction* GetRunAction();
		QAction* GetClearAction();
		QAction* GetNewAction();
		QAction* GetLoadAction();
		QAction* GetSaveAction();
		QAction* GetSaveAsAction();
		QAction* GetQuitAction();
		QTabWidget* GetTabWidget();
		QString OpenLoadDialog();
		QString OpenSaveDialog();

	private:
		int position;
		ExceptionWindow* exceptionWindow;
		std::vector<CodeEditor*> codeEditorVector;
		Highlighter* highlighter;
		QCompleter* completer;
		QTabWidget* tabs;

		QAbstractItemModel *modelFromFile(const QString& fileName);
		CodeEditor* CreateEditor();
		
		ExceptionWindow* CreateExceptionWindow();

		// Menu include all menu items
		QMenu *fileMenu;
		QAction *runAction, 
				*clearAction, 
				*newAction, 
				*openAction, 
				*saveAction,
				*saveAsAction, 
				*quitAction;

		void ShowMenuBar();
};