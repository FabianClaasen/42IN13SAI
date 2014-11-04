#pragma once

#include <QMainWindow>
#include <QObject>
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

	private:
		CodeEditor* codeEditor;
		QString getFileFromStream();
};