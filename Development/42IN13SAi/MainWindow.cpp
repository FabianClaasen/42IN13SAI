#include <QtWidgets>
#include <QShortcut>
#include <qstring.h>
#include <QMessageBox>

#include "MainWindow.h"
#include "CodeEditor.h"

MainWindow::MainWindow(QWidget *parent)
{
	QShortcut *shortcut = new QShortcut(QKeySequence(Qt::Key_F5), this);
	codeEditor = new CodeEditor();
	connect(shortcut, SIGNAL(activated()), this, SLOT(execute()));
	
	this->setCentralWidget(codeEditor);
}

void MainWindow::execute()
{
	QMessageBox msgBox;

	msgBox.setText("Execute called!");

	msgBox.exec();
	isExecuted = true;
}

QString MainWindow::getText()
{
	return codeEditor->toPlainText();
}

MainWindow::~MainWindow()
{
	
}