#include "MainController.h"

MainController::MainController() : QObject()
{
	mainWindow.setWindowTitle(QObject::tr("Short C Editor"));
	mainWindow.resize(640, 360);
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
	connect(mainWindow.GetLoadAction(), SIGNAL(triggered()), this, SLOT(LoadFile()));
	connect(mainWindow.GetSaveAction(), SIGNAL(triggered()), this, SLOT(SaveFile()));
	connect(mainWindow.GetSaveAsAction(), SIGNAL(triggered()), this, SLOT(SaveAsFile()));
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
		puts(e.what());
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
		puts(e.what());
		return;
	}
	// Delete the tokenizer controller
	delete(tokenizer_controller);

	// Run the virtual machine with the compilernodes
	VirtualMachine virtual_machine = VirtualMachine(compiler.GetSymbolTable(), compiler.GetSubroutineTable(), compiler.GetCompilerNodes());
	virtual_machine.ExecuteCode();
}

void MainController::ClearConsole()
{
	system("cls");
}

std::string MainController::GetFileFromStream()
{
	QString gen_code = mainWindow.GetText();

	std::shared_ptr<QFile> file;
	std::shared_ptr<QFile> currentFile = currentFiles.at(mainWindow.GetCurrentTabPosition() - 1);
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

void MainController::LoadFile()
{
	QString URI = mainWindow.OpenLoadDialog();
	QString text = FileIO::LoadFile(URI);

	currentFiles.push_back(std::shared_ptr<QFile>(new QFile(URI)));
	QFileInfo* fileInfo = new QFileInfo(URI);
	mainWindow.AddFile(fileInfo, text);
	//mainWindow.SetText(text);
}

void MainController::SaveFile()
{
	std::shared_ptr<QFile> currentFile = currentFiles.at(mainWindow.GetCurrentTabPosition() - 1);
	if (!currentFile)
	{
		QString URI = mainWindow.OpenSaveDialog();
		FileIO::SaveFile(URI, mainWindow.GetText());
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
}

MainController::~MainController()
{
	// Empty
}