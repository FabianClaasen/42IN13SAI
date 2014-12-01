#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
{
	ShowMenuBar();

	// Set themer
	Themer themer;

	// Set the tabs
	tabs = new QTabWidget();
	tabs->setTabsClosable(true);

	outputWindow = new OutputWindow();
	exceptionWindow = new ExceptionWindow();
	outputTab = new QTabWidget();
	outputTab->addTab(outputWindow, "Output");
	outputTab->addTab(exceptionWindow, "Exceptions");

	//Make a layout to add different widgets
	splitter = new QSplitter();
	splitter->setOrientation(Qt::Vertical);
	splitter->setHandleWidth(10);
	splitter->addWidget(tabs);
	splitter->addWidget(outputTab);

	QList<int> sizes = QList<int>() << 500 << 10;
	splitter->setSizes(sizes);

	this->setCentralWidget(splitter);
}

void MainWindow::ShowMenuBar()
{
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
	quitAction = fileMenu->addAction("Quit");
	runAction = menu->addAction("Run");
	clearAction = menu->addAction("Clear console");

	

#ifndef _WIN32
	// Also needs a menu to show the items, doesn't work with only actions
	QMenu* mainMenu = menu->addMenu("Debug");
	
	// Add the actions to the menu
	mainMenu->addAction(runAction);
	mainMenu->addAction(clearAction);
#endif

	// Add the Theme selection menu
	themeMenu = menu->addMenu("Themes");
	CreateThemeMenu();

	// Set the menu bar on ui from left to right
	this->setMenuBar(menu);
	menu->setLayoutDirection(Qt::LeftToRight);
}

CodeEditor* MainWindow::CreateEditor()
{
	CodeEditor* codeEditor = new CodeEditor();
	themer.AddEditor(codeEditor);

	// Set the completer
	completer = new QCompleter(this);

	QString str = QDir::currentPath();
	str.append("/Resources/words.txt");

	completer->setModel(modelFromFile(str));
	completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
	completer->setCaseSensitivity(Qt::CaseSensitive);
	completer->setWrapAround(false);
	codeEditor->setCompleter(completer);
	codeEditor->setFocus(Qt::OtherFocusReason);
	codeEditor->installEventFilter(this);
	// Set the highlighter
	highlighter = new Highlighter(codeEditor->document());

	return codeEditor;
}

void MainWindow::CreateThemeMenu()
{
	std::vector<std::string> themeNames = themer.GetThemesVector();
	std::string currentTheme = themer.GetCurrentTheme();

	for (std::string themeName : themeNames)
	{
		QAction *action = themeMenu->addAction(themeName.c_str());
		action->setCheckable(true);
		action->setChecked(themeName == currentTheme);
		connect(action, SIGNAL(triggered()), this, SLOT(ChangeTheme()));
	}
}

void MainWindow::ChangeTheme()
{
	for (QAction *action : themeMenu->actions())
	{
		action->setChecked(false);
	}
	QAction* selectedAction = dynamic_cast<QAction*>(sender());
	selectedAction->setChecked(true);
	themer.SetTheme(selectedAction->text().toStdString());
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
	codeEditorVector.erase(codeEditorVector.begin() + index);
	themer.RemoveEditor(index);
}

void MainWindow::AddNewTab()
{
	CodeEditor* codeEditor = CreateEditor();
	themer.AddEditor(codeEditor);
	codeEditorVector.push_back(codeEditor);
	tabs->addTab(codeEditor, "New*");
}

void MainWindow::addOutput(std::string output)
{
	outputTab->setCurrentIndex(0);
	outputWindow->addOutput(output);
}

void MainWindow::addException(std::string exception)
{
	outputTab->setCurrentIndex(1);
	exceptionWindow->addException(exception);
}

void MainWindow::clearExceptions()
{
	exceptionWindow->clearExceptions();
}

void MainWindow::clearOutput()
{
	outputWindow->clearOutput();
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
	return QFileDialog::getOpenFileName(this, tr("Open file"), "", tr("Text Files (*.sc)"));
}

QString MainWindow::OpenSaveDialog()
{
	return QFileDialog::getSaveFileName(this, tr("Save file"), "", tr("Text Files (*.sc)"));
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

void MainWindow::SaveThemeSettings()
{
	themer.SaveCurrentTheme();
}

MainWindow::~MainWindow()
{
	delete menu;
}