#include "MainController.h"

MainController::MainController() : QObject()
{
	mainWindow.setWindowTitle(QObject::tr("Short C Editor"));
	mainWindow.showMaximized();
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

	// Create a first new file
	NewFile();
}

void MainController::Execute()
{
	//Clear the output windows
	mainWindow.clearOutput();
	mainWindow.clearExceptions();

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
	std::list<std::shared_ptr<CompilerNode>> nodesList = compiler.GetCompilerNodes();
	std::shared_ptr<LinkedList> nodesLinkedList = std::make_shared<LinkedList>(nodesList);
	VirtualMachine virtual_machine = VirtualMachine(compiler.GetSymbolTable(), compiler.GetSubroutineTable(), nodesLinkedList);

	try
	{
		virtual_machine.ExecuteCode();
		std::vector<std::string> output = virtual_machine.getOutput();
		for (std::vector<std::string>::iterator it = output.begin(); it != output.end(); ++it) {
			mainWindow.addOutput(*it);
		}

		// Save the output in an output file
		FileIO::SaveFile("output.txt", mainWindow.GetText());
	}
	catch (const std::exception& e)
	{
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
		file = std::make_shared<QFile>("number1.sc");
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
	currentFiles.push_back(std::make_shared<QFile>(nullptr));
	mainWindow.AddNewTab();
}

void MainController::LoadFile()
{
	QString URI = mainWindow.OpenLoadDialog();

	if (!URI.isEmpty())
	{
		QString text = FileIO::LoadFile(URI);

		// Add the loaded file
		currentFiles.push_back(std::shared_ptr<QFile>(new QFile(URI)));
		QFileInfo* fileInfo = new QFileInfo(URI);
		mainWindow.AddFile(fileInfo, text);
	}
}

void MainController::SaveFile()
{
	std::shared_ptr<QFile> currentFile;

	if (currentFiles.size() > mainWindow.GetCurrentTabPosition())
	{
		currentFile = currentFiles.at(mainWindow.GetCurrentTabPosition());
	}

	if (!currentFile)
	{
		QString URI = mainWindow.OpenSaveDialog();
		if (!URI.isEmpty())
		{
			FileIO::SaveFile(URI, mainWindow.GetText());
			QFileInfo* fileInfo = new QFileInfo(URI);
			mainWindow.SetTabTitle(fileInfo);
		}
	}
	else
	{
		FileIO::SaveFile(currentFile, mainWindow.GetText());
	}	
}

void MainController::SaveAsFile()
{
	QString URI = mainWindow.OpenSaveDialog();
	if (!URI.isEmpty())
	{
		FileIO::SaveFile(URI, mainWindow.GetText());
		QFileInfo* fileInfo = new QFileInfo(URI);
		mainWindow.SetTabTitle(fileInfo);
	}
}

void MainController::Quit()
{
	mainWindow.SaveThemeSettings();
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