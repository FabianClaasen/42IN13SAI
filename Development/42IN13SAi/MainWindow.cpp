#include <QtWidgets>
#include <QMenu>

#include "MainWindow.h"
#include "CodeEditor.h"
#include "TokenizerController.h"
#include "Compiler.h"
#include "VirtualMachine.h"
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent)
{
	ShowMenuBar();
	codeEditor = new CodeEditor();

	// set the completer
	completer = new QCompleter(this);
	completer->setModel(modelFromFile("C:\\42IN14SAi\\words.txt"));
	completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
	completer->setCaseSensitivity(Qt::CaseInsensitive);
	completer->setWrapAround(false);
	codeEditor->setCompleter(completer);
	codeEditor->installEventFilter(this);
	// set the highlighter
	highlighter = new Highlighter(codeEditor->document());

	this->setCentralWidget(codeEditor);
}

void MainWindow::ShowMenuBar()
{
	// Create menu bar
	QMenuBar* menu = nullptr;

	#ifdef _WIN32
		menu = new QMenuBar();
	#else
        // Mac OS X needs menubar without a parent
		menu = new QMenuBar(0);
	#endif

	// Create action and connect
	fileMenu = menu->addMenu("File");
	openAction = fileMenu->addAction("Open");
	saveAction = fileMenu->addAction("Save");
	saveAsAction = fileMenu->addAction("Save as");
	runAction = menu->addAction("Run");
	clearAction = menu->addAction("Clear console");

	quitAction = menu->addAction("Quit");

	#ifndef _WIN32
        // Also needs a menu to show the items, doesn't work with only actions
        QMenu* mainMenu = menu->addMenu("Debug");
    
        // Add the actions to the menu
		mainMenu->addAction(runAction);
		mainMenu->addAction(clearAction);
	#endif

	// Set the menu bar on ui from left to right
	this->setMenuBar(menu);
	menu->setLayoutDirection(Qt::LeftToRight);
}

QAction* MainWindow::GetRunAction()
{
	return runAction;
}

QAction* MainWindow::GetClearAction()
{
	return clearAction;
}

QAction* MainWindow::GetLoadAction()
{
	return openAction;
}

QAction* MainWindow::GetSaveAction()
{
	return saveAction;
}

QAction* MainWindow::GetSaveAsAction()
{
	return saveAsAction;
}

QAction* MainWindow::GetQuitAction()
{
	return quitAction;
}

QString MainWindow::GetText()
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

QString MainWindow::OpenLoadDialog()
{
	return QFileDialog::getOpenFileName(this, tr("Open file"), "", tr("Text Files (*.txt)"));
}

QString MainWindow::OpenSaveDialog()
{
	return QFileDialog::getSaveFileName(this, tr("Save file"), "", tr("Text Files (*.txt)"));
}

void MainWindow::SetText(QString text)
{
	codeEditor->clear();
	codeEditor->insertPlainText(text);
}

MainWindow::~MainWindow()
{
	// Keep empty, Qobject is cleaning everything for you :)
}