#include "MainController.h"

MainController::MainController() : QObject()
{
	mainWindow.setWindowTitle(QObject::tr("Short C Editor"));
	mainWindow.resize(1000, 500);
	mainWindow.show();

	Setup();
}

void MainController::Setup()
{
	QShortcut *shortcut = new QShortcut(QKeySequence(Qt::Key_F5), &mainWindow);
	connect(shortcut, SIGNAL(activated()), this, SLOT(Execute()));
    
#ifndef _WIN32
    // Mac OS X cmd + R for running program like in xcode
    QShortcut *macShortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_R), &mainWindow);
    connect(macShortcut, SIGNAL(activated()), this, SLOT(Execute()));
#endif

	connect(mainWindow.GetRunAction(), SIGNAL(triggered()), this, SLOT(Execute()));
	connect(mainWindow.GetClearAction(), SIGNAL(triggered()), this, SLOT(ClearConsole()));
	connect(mainWindow.GetNewAction(), SIGNAL(triggered()), this, SLOT(NewFile()));
	connect(mainWindow.GetLoadAction(), SIGNAL(triggered()), this, SLOT(LoadFile()));
	connect(mainWindow.GetSaveAction(), SIGNAL(triggered()), this, SLOT(SaveFile()));
	connect(mainWindow.GetSaveAsAction(), SIGNAL(triggered()), this, SLOT(SaveAsFile()));
	connect(mainWindow.GetQuitAction(), SIGNAL(triggered()), this, SLOT(Quit()));
	connect(mainWindow.GetTabWidget(), SIGNAL(tabCloseRequested(int)), this, SLOT(CloseTab(int)));
}

void MainController::Execute()
{
	// Excute typed code
	// Get the file from the stream and convert to std::string
	std::string input(GetFileFromStream());

	TokenizerController *tokenizer_controller = new TokenizerController(input);

	try
	{
		tokenizer_controller->Tokenize(); // Tokenize
	}
	catch (const std::exception& e)
	{
		delete(tokenizer_controller);
		mainWindow.addException(e.what());
		return;
	}

	// Run the compiler
	Compiler compiler = Compiler(tokenizer_controller->GetCompilerTokens());

	try
	{
		compiler.Compile();
	}
	catch (const std::exception& e)
	{
		delete(tokenizer_controller);
		mainWindow.addException(e.what());
		return;
	}
	// Delete the tokenizer controller
	delete(tokenizer_controller);

	// Run the virtual machine with the compilernodes
	VirtualMachine virtual_machine = VirtualMachine(compiler.GetSymbolTable(), compiler.GetSubroutineTable(), compiler.GetCompilerNodes());

	try
	{
		virtual_machine.ExecuteCode();
	}
	catch (const std::exception& e)
	{
		delete(tokenizer_controller);
		mainWindow.addException(e.what());
		return;
	}
}

void MainController::ClearConsole()
{
	system("cls");
}

std::string MainController::GetFileFromStream()
{
	QString gen_code = mainWindow.GetText();

	std::shared_ptr<QFile> file;
	std::shared_ptr<QFile> currentFile;
	if (currentFiles.size() > mainWindow.GetCurrentTabPosition() - 1)
	{
		currentFile = currentFiles.at(mainWindow.GetCurrentTabPosition() - 1);
	}

	if (currentFile)
	{
		file = currentFile;
	}
	else
	{
		file = std::shared_ptr<QFile>(new QFile("number1.txt"));
	}

	if (file->exists())
	{
		file->remove();
	}

	file->open(QIODevice::ReadWrite | QIODevice::Text);
	QTextStream stream(file.get());

	stream << gen_code;
	file->close();

	QFileInfo info(*file);
	QString filepath = info.absoluteFilePath();

	std::string filepath_std = filepath.toLocal8Bit().constData();
	return filepath_std;
}

void MainController::NewFile()
{
	mainWindow.AddNewTab();
}

void MainController::LoadFile()
{
	QString URI = mainWindow.OpenLoadDialog();
	QString text = FileIO::LoadFile(URI);

	// Check if the first file is not the start file
	if (currentFiles.size() <= 0)
		mainWindow.RemoveTab(0);
	
	// Add the loaded file
	currentFiles.push_back(std::shared_ptr<QFile>(new QFile(URI)));
	QFileInfo* fileInfo = new QFileInfo(URI);
	mainWindow.AddFile(fileInfo, text);
}

void MainController::SaveFile()
{
	std::shared_ptr<QFile> currentFile;
	if (currentFiles.size() > mainWindow.GetCurrentTabPosition() - 1)
	{
		currentFile = currentFiles.at(mainWindow.GetCurrentTabPosition() - 1);
	}

	if (!currentFile)
	{
		QString URI = mainWindow.OpenSaveDialog();
		FileIO::SaveFile(URI, mainWindow.GetText());
		QFileInfo* fileInfo = new QFileInfo(URI);
		mainWindow.SetTabTitle(fileInfo);
	}
	else
	{
		FileIO::SaveFile(currentFile, mainWindow.GetText());
	}	
}

void MainController::SaveAsFile()
{
	QString URI = mainWindow.OpenSaveDialog();
	FileIO::SaveFile(URI, mainWindow.GetText());
	QFileInfo* fileInfo = new QFileInfo(URI);
	mainWindow.SetTabTitle(fileInfo);
}

void MainController::Quit()
{
	std::exit(0);
}

void MainController::CloseTab(int index)
{
	mainWindow.RemoveTab(index);
	
	// Remove the file on the current position
	if (currentFiles.size() > index)
		currentFiles.erase(currentFiles.begin() + index);
}

MainController::~MainController()
{
	// Empty
}