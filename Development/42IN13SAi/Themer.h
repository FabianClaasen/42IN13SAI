#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>

#include <qdir.h>
#include <boost\filesystem.hpp>
#include "jsoncpp\json.h"

#include "Highlighter.h"
#include "CodeEditor.h"

namespace fs = boost::filesystem;
class Themer
{
public:
	Themer();
	virtual ~Themer();

	void AddEditor(CodeEditor* codeEditor);
	void RemoveEditor(int index);
	void SetTheme(std::string themeName);
	void LoadThemes();
	std::vector<std::string> GetThemesVector();
	std::string GetCurrentTheme();
private:
	bool userThemeSet = false;
	std::string defaultTheme;
	std::string userTheme;
	std::string currentThemeName;
	Json::Value currentTheme;
	std::map<std::string, std::string> themes;

	// Theme settings
	std::map<std::string, QColor> colors;
	std::string fontFamily;
	int fontSize;

	// editors and highlighter
	std::vector<CodeEditor*> editors;
	std::unique_ptr<Highlighter> highlighter;

	// load functions
	void LoadTheme(fs::path filePath);
	void LoadSettings();

	// set the editor and highlighter themes
	void SetEditors();
	void SetEditor(CodeEditor* editor);

	std::string getFileString(std::string filePath);
};

