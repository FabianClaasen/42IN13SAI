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

#include "Highlighter.h"
#include "CodeEditor.h"

namespace fs = boost::filesystem;
class Themer
{
public:
	Themer();
    virtual ~Themer();
    
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
private:
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
    std::string fontFamily;
    int fontSize;
    
    // editors and highlighter
    std::vector<CodeEditor*> editors;
    std::vector<Highlighter *> highlighters;
    
    // load functions
    void LoadTheme(fs::path filePath);
    void LoadSettings();
    
    // Editor style setters
    void SetEditors();
    void SetEditor(CodeEditor* editor);
    void SetEditorStyles(Json::Value editorStyles);
    
    // Highlighter style setters
    void SetHighlighters();
    void SetHighlighter(CodeEditor* editor);
    void SetHighlighterStyles(Json::Value highlighterStyles);
    
    
    std::string getFileString(std::string filePath);
};

