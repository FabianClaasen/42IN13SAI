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

		QMenu *fileMenu;
		QAction *runAction;
		QAction *clearAction;
		QAction *openAction;
		QAction *saveAction;
		QAction *saveAsAction;

		void ShowMenuBar();
};