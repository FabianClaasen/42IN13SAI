#include "MainController.h"

MainController::MainController() : QObject()
{
    mainWindow.setWindowTitle(QObject::tr("Short C Editor"));
    mainWindow.showMaximized();
    Setup();
}

void MainController::Setup()
{
    QString resourceDir = QDir::currentPath().append("/Resources/");
#ifndef _WIN32
    resourceDir = QCoreApplication::applicationDirPath() + "/";
#endif

    // Add default font to the font database
    QFontDatabase fontDatabase;
    fontDatabase.addApplicationFont(resourceDir + "DejaVuSansMono.ttf");
    fontDatabase.addApplicationFont(resourceDir + "DejaVuSansMono-Bold.ttf");
    fontDatabase.addApplicationFont(resourceDir + "DejaVuSansMono-Oblique.ttf");

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
    // Multithreading code
    if (virtual_machine == nullptr)
    {
        //Clear the output windows
        mainWindow.clearOutput();
        mainWindow.clearExceptions();

        // Setup output buffer
        bio::stream_buffer<ExceptionOutput> sb;
        sb.open(ExceptionOutput(this));
        std::clog.rdbuf(&sb);

        // Excute typed code
        // Get the file from the stream and convert to std::string
        std::string input(GetFileFromStream());
        tokenizer_controller = std::make_shared<TokenizerController>(input);

        try
        {
            // Tokenize
            tokenizer_controller->Tokenize();
        }
        catch (const std::exception& e)
        {
            mainWindow.addException(e.what());
            return;
        }

        // Tokenizer has exceptions stop the build
        if (tokenizer_controller->HasExceptions())
            return;

        // Run the compiler
        compiler = std::make_shared<Compiler>(tokenizer_controller->GetCompilerTokens());

        try
        {
            // Compile
            compiler->Compile();
        }
        catch (const std::exception& e)
        {
            mainWindow.addException(e.what());
            return;
        }

        // Compiler has exceptions stop the build
        if (compiler->HasExceptions())
            return;

        // Run the virtual machine with the compilernodes
        std::list<std::shared_ptr<CompilerNode>> nodesList = compiler->GetCompilerNodes();
        std::shared_ptr<LinkedList> nodesLinkedList = std::make_shared<LinkedList>(nodesList);

        virtual_machine = std::make_shared<VirtualMachine>(compiler->GetSymbolTable(), compiler->GetSubroutineTable(), nodesLinkedList);
        connect(virtual_machine.get(), &VirtualMachine::PrintOutput, this, &MainController::PrintOutput);
        connect(virtual_machine.get(), &VirtualMachine::PrintException, this, &MainController::PrintException);
        connect(virtual_machine.get(), &VirtualMachine::Finished, this, &MainController::VirtualMachineFinished);

        try
        {
            // Create the log files directory if it doesn't exists
            if (!QDir("Log files").exists())
                QDir().mkdir("Log files");

            // Execute VM
            virtual_machine->start();

            // Start timer to print output
			output_thread = new std::thread([&](){
                output_thread_running = true;

                start_time = std::clock();
                while (output_thread_running)
                {
                    if ((std::clock() - start_time) > 250)
                    {
                        mainWindow.PrintOutput();
                        start_time = std::clock();
                    }
                }
            });

            mainWindow.CodeIsExecuting(true);
        }
        catch (const std::exception& e)
        {
            mainWindow.addException(e.what());
            return;
        }
    }
    else
    {
        if (dialog == nullptr)
            dialog = new StopExecuteDialog();

        connect(dialog->GetCancelButton(), SIGNAL(released()), this, SLOT(HideDialog()));
        connect(dialog->GetOkButton(), SIGNAL(released()), this, SLOT(StopVirtualMachine()));
        dialog->show();
    }
}

void MainController::PrintOutput(QString output)
{
    mainWindow.addOutput(output.toStdString());
    this->output.append(output + "\n");
}

void MainController::PrintException(QString exception)
{
    mainWindow.addException(exception.toStdString());
}

void MainController::StopVirtualMachine()
{
    virtual_machine->quit();
    HideDialog();

    // Stop the output thread and print output
    output_thread_running = false;
    mainWindow.PrintOutput();

    // Save the output in an output file
    FileIO::SaveFile("Log files//output.txt", this->output);
}

void MainController::VirtualMachineFinished()
{
    tokenizer_controller = nullptr;
    compiler = nullptr;
    virtual_machine = nullptr;
    mainWindow.CodeIsExecuting(false);

    // Stop the output thread and print output
    output_thread_running = false;
    mainWindow.PrintOutput();

    // Save the output in an output file
    FileIO::SaveFile("Log files//output.txt", this->output);
}

void MainController::HideDialog()
{
    if (dialog != nullptr)
        dialog->hide();
}

void MainController::CloseErrorDialog()
{
    if (error_dialog != nullptr)
        error_dialog->hide();
}

void MainController::WriteException(const char* output, std::streamsize size)
{
    std::string exception_text;
    for (int i = 0; i < size; i++)
        exception_text.append(1, output[i]);
    mainWindow.addException(exception_text);
    //this->output.append(QString::fromUtf8(exception_text.c_str()));
}

void MainController::ClearConsole()
{
#ifdef _WIN32
    system("cls");
#endif

    mainWindow.clearOutput();
}

std::string MainController::GetFileFromStream()
{
    QString gen_code = mainWindow.GetText();

    std::shared_ptr<QFile> file;
    std::shared_ptr<QFile> currentFile = nullptr;
    if (currentFiles.size() > mainWindow.GetCurrentTabPosition())
    {
        currentFile = currentFiles.at(mainWindow.GetCurrentTabPosition());
    }

    //currentFile
    if (!currentFile->fileName().isEmpty() && currentFile->fileName().endsWith(".cs"))
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

        if (file->error())
            std::cout << file->errorString().toStdString() << std::endl;
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
    try
    {
        QString URI = mainWindow.OpenLoadDialog();

        if (!URI.isEmpty() && URI.endsWith(".sc"))
        {
            QString text = FileIO::LoadFile(URI);

            // Add the loaded file
            currentFiles.push_back(std::shared_ptr<QFile>(new QFile(URI)));
            QFileInfo* fileInfo = new QFileInfo(URI);
            mainWindow.AddFile(fileInfo, text);
            delete fileInfo;
        }
        else if(!(URI == "canceled"))
            throw LoadException("You can only load .sc files");
    }
    catch (LoadException e)
    {
        error_dialog = new ErrorDialog(e.what());
        connect(error_dialog->GetCloseButton(), SIGNAL(released()), this, SLOT(CloseErrorDialog()));
        error_dialog->show();
    }
}

void MainController::SaveFile()
{
    try
    {
        std::shared_ptr<QFile> currentFile;

        if (currentFiles.size() > mainWindow.GetCurrentTabPosition())
        {
            currentFile = currentFiles.at(mainWindow.GetCurrentTabPosition());
        }

        if (!currentFile->fileName().endsWith(".sc"))
        {
            SaveAsFile();
        }
        else
        {
            FileIO::SaveFile(currentFile, mainWindow.GetText());
        }
    }
    catch (SaveException e)
    {
        error_dialog = new ErrorDialog(e.what());
        connect(error_dialog->GetCloseButton(), SIGNAL(released()), this, SLOT(CloseErrorDialog()));
        error_dialog->show();
    }
}

void MainController::SaveAsFile()
{
    try
    {
        QString URI = mainWindow.OpenSaveDialog();
		if (!URI.isEmpty() && URI != "canceled")
        {
            FileIO::SaveFile(URI, mainWindow.GetText());
            QFileInfo* fileInfo = new QFileInfo(URI);
			mainWindow.SetTabTitle(fileInfo);
            delete fileInfo;
        }
        else if(!(URI == "canceled"))
            throw SaveException("You can only save the file as a .sc file");
    }
    catch (SaveException e)
    {
        error_dialog = new ErrorDialog(e.what());
        connect(error_dialog->GetCloseButton(), SIGNAL(released()), this, SLOT(CloseErrorDialog()));
        error_dialog->show();
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
    //delete output_thread;
}
