#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>

#include <QDir>
#include <QCoreApplication>
#include <QByteArray>
#include <boost/filesystem.hpp>
#include "jsoncpp/json.h"

#include "CodeEditor.h"
#include "Highlighter.h"
#include "OutputWindow.h"
#include "ExceptionWindow.h"

class MainWindow;

namespace fs = boost::filesystem;
class Themer
{
public:
    Themer();
	Themer(MainWindow* p_main);
    virtual ~Themer();
    
	// public main & output -window setter
	void SetMainWindow(MainWindow* mainWindow);
	void SetOutputWindow(OutputWindow* outputWindow);
	void SetExceptionWindow(ExceptionWindow* exceptionWindow);

    // public editor addition and removal
    void AddEditor(CodeEditor* codeEditor);
    void RemoveEditor(int index);
    
    // public theme functions
    void SetTheme(std::string themeName);
    void LoadThemes();
    void SaveCurrentTheme();
    
    // Get theme info for menu
    std::vector<std::string> GetThemesVector();
    std::string GetCurrentTheme();
    std::map<std::string, QColor> GetWindowStyles();
private:
    MainWindow* main;
    
	bool userThemeSet = false;
	std::string defaultTheme;
	std::string userTheme;
    
	Json::Value settings;
	std::string settingsPath;
    std::string themesPath;
	
    std::string currentThemeName;
	Json::Value currentTheme;
	std::map<std::string, std::string> themes;

    // Theme settings
    std::map<std::string, QColor> colors;
    std::map<std::string, QColor> syntaxColors;
    std::map<std::string, QColor> mainColors;
	std::map<std::string, QColor> outputWindowColors;
	std::map<std::string, QColor> exceptionWindowColors;
    std::string fontFamily;
    int fontSize;
    
    // editors, mainwindow, outputwindow and highlighter
    std::vector<CodeEditor*> editors;
    std::vector<Highlighter *> highlighters;

	MainWindow* mainWindow;
	OutputWindow* outputWindow;
	ExceptionWindow* exceptionWindow;
    
    // load functions
    void LoadTheme(fs::path filePath);
    void LoadSettings();
    
    // Main window style setters
    void SetWindowStyles(Json::Value mainStyles);
    
    // Editor style setters
    void SetEditors();
    void SetEditor(CodeEditor* editor);
    void SetEditorStyles(Json::Value editorStyles);

	// Output & exception style setters
	void SetOutputWindowStyles(Json::Value outputWindowStyles);
	void SetExceptionWindowStyles(Json::Value exceptionWindowStyles);
    
    // Highlighter style setters
    void SetHighlighters();
    void SetHighlighter(CodeEditor* editor);
    void SetHighlighterStyles(Json::Value highlighterStyles);
    
    
    std::string getFileString(std::string filePath);
};

