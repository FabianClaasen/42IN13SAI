#include <QtWidgets>
#include <QMenu>

#include "MainWindow.h"
#include "CodeEditor.h"
#include "TokenizerController.h"
#include "Compiler.h"
#include "VirtualMachine.h"
#include <QKeyEvent>
#include "CodeCompleter.h"

MainWindow::MainWindow(QWidget *parent)
{
	showMenuBar();
	codeEditor = new CodeEditor();
	highlighter = new Highlighter(codeEditor->document());

	completer = new QCompleter(this);
	completer->setModel(modelFromFile("C:\\Users\\stefan\\Desktop\\words.txt"));
	completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
	completer->setCaseSensitivity(Qt::CaseInsensitive);
	completer->setWrapAround(false);
	codeEditor->setCompleter(completer);

	this->setCentralWidget(codeEditor);
}

void MainWindow::showMenuBar()
{
	// Create menu bar
	QMenuBar* menu = nullptr;

	#ifdef _WIN32
		menu = new QMenuBar();
	#else
		menu = new QMenuBar(0);

		QMenu* mainMenu = menu->addMenu("File");
	#endif

	// Create action and connect
	runAction = menu->addAction("Run");
	clearAction = menu->addAction("Clear console");

	#ifndef _WIN32
		mainMenu->addAction(runAction);
		mainMenu->addAction(clearAction);
	#endif

	// Set the menu bar on ui from left to right
	this->setMenuBar(menu);
	menu->setLayoutDirection(Qt::LeftToRight);
}

QAction *MainWindow::getRunAction()
{
	return runAction;
}

QAction *MainWindow::getClearAction()
{
	return clearAction;
}

QString MainWindow::getText()
{
	return codeEditor->toPlainText();
}

QAbstractItemModel* MainWindow::modelFromFile(const QString& fileName)
{
	QFile file(fileName);
	if (!file.open(QFile::ReadOnly))
		return new QStringListModel(completer);

#ifndef QT_NO_CURSOR
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
#endif
	QStringList words;

	while (!file.atEnd()) {
		QByteArray line = file.readLine();
		if (!line.isEmpty())
			words << line.trimmed();
	}

#ifndef QT_NO_CURSOR
	QApplication::restoreOverrideCursor();
#endif
	return new QStringListModel(words, completer);
}

//void MainWindow::keyPressEvent(QKeyEvent* event)
//{
//	std::cout << "pressed in mainwindow " << event->key() << std::endl;
//	//CodeCompleter::keyPressEvent(event);
//}

MainWindow::~MainWindow()
{
	// Empty
}