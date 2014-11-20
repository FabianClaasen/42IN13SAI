#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
{
	ShowMenuBar();

	// Create code editor
	CodeEditor* codeEditor = CreateEditor();
	codeEditorVector.push_back(codeEditor);

	// Set the tabs
	tabs = new QTabWidget();
	tabs->setTabsClosable(true);
	tabs->addTab(codeEditor, "New*");

	exceptionWindow = CreateExceptionWindow();

	//Make a layout to add different widgets
	QVBoxLayout* layout = new QVBoxLayout();
	layout->addWidget(tabs);
	layout->addWidget(exceptionWindow);

	//make a central widget set the layout and add it on the mainwindow
	QWidget* mainWidget = new QWidget();
	mainWidget->setLayout(layout);
	this->setCentralWidget(mainWidget);
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
	newAction = fileMenu->addAction("New");
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

CodeEditor* MainWindow::CreateEditor()
{
	CodeEditor* codeEditor = new CodeEditor();

	// Set the completer
	completer = new QCompleter(this);
	completer->setModel(modelFromFile("C:\\42IN14SAi\\words.txt"));
	completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
	completer->setCaseSensitivity(Qt::CaseInsensitive);
	completer->setWrapAround(false);
	codeEditor->setCompleter(completer);
	codeEditor->installEventFilter(this);

	// Set the highlighter
	highlighter = new Highlighter(codeEditor->document());

	return codeEditor;
}

ExceptionWindow* MainWindow::CreateExceptionWindow()
{
	ExceptionWindow* exceptionWindow = new ExceptionWindow();

	return exceptionWindow;
}

QAction* MainWindow::GetRunAction()
{
	return runAction;
}

QAction* MainWindow::GetClearAction()
{
	return clearAction;
}

QAction* MainWindow::GetNewAction()
{
	return newAction;
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

QTabWidget* MainWindow::GetTabWidget()
{
	return tabs;
}

int MainWindow::GetCurrentTabPosition()
{
	return tabs->currentIndex();
}

void MainWindow::RemoveTab(int index)
{
	tabs->removeTab(index);
}

void MainWindow::AddNewTab()
{
	tabs->addTab(CreateEditor(), "New*");
}

void MainWindow::addException(std::string exception)
{
	exceptionWindow->addException(exception);
}

void MainWindow::SetTabTitle(QFileInfo* info)
{
	tabs->setTabText(tabs->currentIndex(), info->baseName());
}

QAction* MainWindow::GetQuitAction()
{
	return quitAction;
}

QString MainWindow::GetText()
{
	CodeEditor* codeEditor = codeEditorVector.at(tabs->currentIndex());
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

void MainWindow::AddFile(QFileInfo* info, QString text)
{
	// Create code editor
	CodeEditor* codeEditor = CreateEditor();
	codeEditor->setPlainText(text);
	codeEditorVector.push_back(codeEditor);

	// Remove tab if only the new file is existing...
	tabs->addTab(codeEditor, info->baseName());
}

MainWindow::~MainWindow()
{
	// Keep empty, Qobject is cleaning everything for you :)
}