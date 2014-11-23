#pragma once

#include <QObject>
#include <QDialog>
#include <QTabWidget>
#include <QFileInfo>

#include "CodeEditor.h"

class TabView : public QTabWidget
{
	Q_OBJECT

public:
	TabView(QWidget *parent = 0);
	~TabView();
};

