#include "Themer.h"
#include "MainWindow.h"

Themer::Themer()
{
    settingsPath = QDir::currentPath().append("/Resources/settings.json").toLocal8Bit().constData();
#ifndef _WIN32
    settingsPath = QCoreApplication::applicationDirPath().toStdString() + "/settings.json";
#endif
    
    LoadThemes();
    LoadSettings();
}

Themer::Themer(MainWindow* p_main)
{
    main = p_main;
    
	//std::string newPath = QDir::currentPath().append("/Resources/settings.json").toLocal8Bit().constData();
	settingsPath = QDir::currentPath().append("/Resources/settings.json").toLocal8Bit().constData();
#ifndef _WIN32
    settingsPath = QCoreApplication::applicationDirPath().toStdString() + "/settings.json";
#endif
    
	LoadThemes();
	LoadSettings();
}

Themer::~Themer()
{
}

void Themer::SetOutputWindow(OutputWindow* outputWindow)
{
	outputWindow->SetTheme(outputWindowColors);

	this->outputWindow = outputWindow;
}

void Themer::SetExceptionWindow(ExceptionWindow* exceptionWindow)
{
	exceptionWindow->SetTheme(exceptionWindowColors);

	this->exceptionWindow = exceptionWindow;
}

void Themer::AddEditor(CodeEditor* codeEditor)
{
	editors.push_back(codeEditor);
	SetEditor(editors.back());
}

void Themer::RemoveEditor(int index)
{
	editors.erase(editors.begin() + index);
}

void Themer::SaveCurrentTheme()
{
	settings["user_theme_set"] = true;
	settings["user_theme"] = currentThemeName;

	// Create styled writer for fancy output
	Json::StyledWriter writer;

	// Save the settings in the file
	std::ofstream settingsFile;
	settingsFile.open(settingsPath);
	settingsFile << writer.write(settings);
	settingsFile.close();
}

void Themer::SetTheme(std::string themeName)
{
    if (themes.count(themeName))
    {
        currentThemeName = themeName;
    }
    // Load the theme from file
    std::string themeString = getFileString(themes[currentThemeName]);
    
    // Create the JSON document
    Json::Reader jsonReader;
    
    jsonReader.parse(themeString, currentTheme);
    Json::Value themeStyles = currentTheme["theme"];
    
    // Set main window node and call the function
    Json::Value mainStyles = themeStyles["window"];
    SetWindowStyles(mainStyles);
    
    // Set editor node and call the function
    Json::Value editorStyles = themeStyles["editor"];
    SetEditorStyles(editorStyles);
    
	// Set output/exception node and call the function
	Json::Value outputStyles = themeStyles["output"];
	Json::Value exceptionStyles = themeStyles["exception"];

	SetOutputWindowStyles(outputStyles);
	SetExceptionWindowStyles(exceptionStyles);

    // Set highlighter node and call the function
    Json::Value highlighterStyles = themeStyles["syntax"];
    SetHighlighterStyles(highlighterStyles);
    
    SetEditors();
    SetHighlighters();
}

void Themer::SetWindowStyles(Json::Value mainStyles)
{
    mainColors.clear();
    
    // Set the background color
    Json::Value bgColors = mainStyles["background_color"];
    QColor bg(bgColors[0].asInt(), bgColors[1].asInt(), bgColors[2].asInt());
    mainColors.insert(std::map<std::string, QColor>::value_type("background", bg));
}

void Themer::SetEditorStyles(Json::Value editorStyles)
{
    // Create the color map
    colors.clear();
    
    // Set the background QColor and add to map
    Json::Value bgColors = editorStyles["background_color"];
    QColor bg(bgColors[0].asInt(), bgColors[1].asInt(), bgColors[2].asInt());
    colors.insert(std::map<std::string, QColor>::value_type("background", bg));
    
    // Set the text QColor and add to map
    Json::Value textColors = editorStyles["text_color"];
    QColor text(textColors[0].asInt(), textColors[1].asInt(), textColors[2].asInt());
    colors.insert(std::map<std::string, QColor>::value_type("text", text));
    
    // Set the higlight QColor and add to map
    Json::Value higlightColors = editorStyles["selection_color"];
    QColor higlight(higlightColors[0].asInt(), higlightColors[1].asInt(), higlightColors[2].asInt());
    colors.insert(std::map<std::string, QColor>::value_type("higlight", higlight));
    
    // Set the highlighted text QColor and add to map
    Json::Value higlighttColors = editorStyles["selection_text_color"];
    QColor highlighted_text(higlighttColors[0].asInt(), higlighttColors[1].asInt(), higlighttColors[2].asInt());
    colors.insert(std::map<std::string, QColor>::value_type("highlighted_text", highlighted_text));
    
    // Set the linenumber bar background QColor and add to map
    Json::Value numberbarColors = editorStyles["linenumber_bar_color"];
    QColor linenumber_background(numberbarColors[0].asInt(), numberbarColors[1].asInt(), numberbarColors[2].asInt());
    colors.insert(std::map<std::string, QColor>::value_type("linenumber_background", linenumber_background));
    
    // Set the linenumber bar text QColor and add to map
    Json::Value numberColors = editorStyles["linenumber_text_color"];
    QColor linenumber_text(numberColors[0].asInt(), numberColors[1].asInt(), numberColors[2].asInt());
    colors.insert(std::map<std::string, QColor>::value_type("linenumber_text", linenumber_text));
    
    // Set the line highlighter QColor and add to map
    Json::Value lineColors = editorStyles["current_line_highlighter_color"];
    QColor currentline_background(lineColors[0].asInt(), lineColors[1].asInt(), lineColors[2].asInt());
    colors.insert(std::map<std::string, QColor>::value_type("currentline_background", currentline_background));
    
    // Set the Font family
    fontFamily = editorStyles["font_family"].asString();
    
    // Set the font size
    fontSize = editorStyles["font_size"].asInt();
}

void Themer::SetHighlighterStyles(Json::Value highlighterStyles)
{
    syntaxColors.clear();
    
    // Return types colors
    Json::Value returnColors = highlighterStyles["return_types"];
    QColor return_types(returnColors[0].asInt(), returnColors[1].asInt(), returnColors[2].asInt());
    syntaxColors.insert(std::map<std::string, QColor>::value_type("return_types", return_types));
    
    // Identifier colors
    Json::Value identColors = highlighterStyles["identifiers"];
    QColor identifiers(identColors[0].asInt(), identColors[1].asInt(), identColors[2].asInt());
    syntaxColors.insert(std::map<std::string, QColor>::value_type("identifiers", identifiers));
    
    // Function colors
    Json::Value functionsColors = highlighterStyles["functions"];
    QColor functions(functionsColors[0].asInt(), functionsColors[1].asInt(), functionsColors[2].asInt());
    syntaxColors.insert(std::map<std::string, QColor>::value_type("functions", functions));
    
    // Operators colors
    Json::Value operatorsColors = highlighterStyles["operators"];
    QColor operators(operatorsColors[0].asInt(), operatorsColors[1].asInt(), operatorsColors[2].asInt());
    syntaxColors.insert(std::map<std::string, QColor>::value_type("operators", operators));
    
    // Variable type colors
    Json::Value variableColors = highlighterStyles["variable_types"];
    QColor variable_types(variableColors[0].asInt(), variableColors[1].asInt(), variableColors[2].asInt());
    syntaxColors.insert(std::map<std::string, QColor>::value_type("variable_types", variable_types));

	// Comment colors
	Json::Value commentColors = highlighterStyles["comments"];
	QColor comments(commentColors[0].asInt(), commentColors[1].asInt(), commentColors[2].asInt());
	syntaxColors.insert(std::map<std::string, QColor>::value_type("comments", comments));
}

void Themer::SetOutputWindowStyles(Json::Value outputStyles)
{
	outputWindowColors.clear();

	// Set the background QColor and add to map
	Json::Value bgColors = outputStyles["background_color"];
	QColor bg(bgColors[0].asInt(), bgColors[1].asInt(), bgColors[2].asInt());

	Json::Value textColors = outputStyles["text_color"];
	QColor txt(textColors[0].asInt(), textColors[1].asInt(), textColors[2].asInt());

	outputWindowColors.insert(std::map<std::string, QColor>::value_type("background", bg));
	outputWindowColors.insert(std::map<std::string, QColor>::value_type("text", txt));
}

void Themer::SetExceptionWindowStyles(Json::Value exceptionStyles)
{
	exceptionWindowColors.clear();

	// Set the background QColor and add to map
	Json::Value bgColors = exceptionStyles["background_color"];
	QColor bg(bgColors[0].asInt(), bgColors[1].asInt(), bgColors[2].asInt());

	Json::Value textColors = exceptionStyles["text_color"];
	QColor txt(textColors[0].asInt(), textColors[1].asInt(), textColors[2].asInt());

	exceptionWindowColors.insert(std::map<std::string, QColor>::value_type("background", bg));
	exceptionWindowColors.insert(std::map<std::string, QColor>::value_type("text", txt));
}

std::map<std::string, QColor> Themer::GetWindowStyles()
{
    return mainColors;
}

void Themer::SetHighlighters()
{
    // create vector for new Highlighters
    std::vector<Highlighter*> newHighlighters;
    for (Highlighter* highlighter : highlighters)
    {
        // Get the document out of the highlighter
        QTextDocument* document = highlighter->document();
        newHighlighters.push_back(new Highlighter(syntaxColors, document));
    }
    // Replace the highlighters with the new highlighters
    highlighters = newHighlighters;
}

void Themer::SetHighlighter(CodeEditor* editor)
{
    // Push the new highlighter
    highlighters.push_back(new Highlighter(syntaxColors, editor->document()));
}

void Themer::SetEditors()
{
    // Loop through editors and set the styles
    for (CodeEditor* editor : editors)
    {
        editor->SetTheme(colors, fontFamily, fontSize);
    }
}

void Themer::SetEditor(CodeEditor* editor)
{
    // Set the style for only one editor
    editor->SetTheme(colors, fontFamily, fontSize);
    SetHighlighter(editor);
}

std::vector<std::string> Themer::GetThemesVector()
{
	std::vector<std::string> themeNames;

	for (std::map<std::string, std::string>::iterator itr = themes.begin(); itr != themes.end(); ++itr)
	{
		themeNames.push_back(itr->first);
	}

	return themeNames;
}

std::string Themer::GetCurrentTheme()
{
	return currentThemeName;
}

void Themer::LoadSettings()
{
	if (fs::exists(settingsPath))
	{
		// Get the settings file contents
		std::string settingsString = getFileString(settingsPath);

		// Parse it to a JSON document
		Json::Reader reader;

		bool parseSucces = reader.parse(settingsString, settings);
		if (parseSucces)
		{
			userThemeSet = settings["user_theme_set"].asBool();
			if (userThemeSet)
			{
				userTheme = settings["user_theme"].asString();
				SetTheme(userTheme);
			}
			else
			{
				SetTheme(settings["default_theme"].asString());
			}
		}
	}
	else
	{
		// Write the data to the root
		settings["default_theme"] = "Solarized Light";
		settings["user_theme_set"] = false;
		settings["user_theme"] = "";

		// Create styled writer for fancy output
		Json::StyledWriter writer;

		// Save the settings in the file
		std::ofstream settingsFile;
		settingsFile.open(settingsPath);
		settingsFile << writer.write(settings);
		settingsFile.close();

		// Call the settings again for loading
		LoadSettings();
	}
}

void Themer::LoadThemes()
{
	themesPath = QDir::currentPath().append("/Resources/Themes").toLocal8Bit().constData();

#ifdef __APPLE__
    themesPath = QCoreApplication::applicationDirPath().toStdString() + "/Themes";
#endif

	if (fs::exists(themesPath))
	{
		// create end iterator to check against
		fs::directory_iterator end_itr;

		// Loop through all the files in the directory
		for (fs::directory_iterator itr(themesPath); itr != end_itr; ++itr)
		{
			std::string extension = itr->path().extension().string();
			if (itr->path().extension() == ".shortc-theme")
			{
				LoadTheme(itr->path());
			}
		}
	}
}

void Themer::LoadTheme(fs::path filePath)
{
	// Load the file into a string
	std::string themeString = getFileString(filePath.string());

	// Parse the theme json
	Json::Value root;
	Json::Reader reader;

	bool parseSuccess = reader.parse(themeString, root);
	if (parseSuccess)
	{
		themes.insert(std::map<std::string, std::string>::value_type(root["theme_name"].asString(), filePath.string()));
	}
}

std::string Themer::getFileString(std::string filePath)
{
	std::ifstream file(filePath);
	std::stringstream fileString;
    fileString << file.rdbuf();
    file.close();
	std::string stringFile = fileString.str();
	fileString.clear();

	return stringFile;
}

