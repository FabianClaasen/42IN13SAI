#pragma once

#include <QMainWindow>
#include <QObject>

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
		QAction *runAction;
		QAction *clearAction;

		void showMenuBar();
};