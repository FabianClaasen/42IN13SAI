#include "OutputWindow.h"
#include <iostream>

OutputWindow::OutputWindow(QWidget *parent) : QListView(parent)
{
	this->setFont(QFont("Consolas", 9));
#ifndef _WIN32
	// Set font to bigger size for readability on Mac OS X
	this->setFont(QFont("Consolas", 12));
#endif

	setEditTriggers(QAbstractItemView::NoEditTriggers);
	setSelectionMode(QAbstractItemView::NoSelection);
	scrollToBottom();

	listModel = new QStringListModel(output, nullptr);
    
	setModel(listModel);
}

void OutputWindow::SetTheme(std::map<std::string, QColor> colors)
{
	// Output colours
	QPalette pallete;
	pallete.setColor(QPalette::Active, QPalette::Base, colors["background"]);
	pallete.setColor(QPalette::Text, colors["text"]);

	this->setPalette(pallete);
}

void OutputWindow::PrintOutput()
{
	QMutexLocker locker(&mutex);
	listModel->setStringList(output);
	this->scrollToBottom();
}

void OutputWindow::addOutput(std::string strOutput)
{
	QMutexLocker locker(&mutex);
	bool is_number = true;
	try {
		std::stof(strOutput);
	}
	catch (const std::exception& e)
	{
		is_number = false;
	}

	QString str = QString::fromUtf8(strOutput.c_str());
	// Algoritm for output precision
	if (is_number)
	{
		str = setOutputPrecision(str);
	}
	output << str;

	// Remove the first item of the output if the size is bigger than 75
	if (output.size() > 500)
		output.removeFirst();
}

QString OutputWindow::setOutputPrecision(QString str)
{
	std::vector<int> charsToRemove;
	for (int i = 0; i < str.count(); i++)
	{
		if (str[i] == '.')
		{
			int k = i + 1;
			for (int j = k; j < str.count(); j++)
			{
				if (str[j] == '0' || str[j] == '\n')
					charsToRemove.push_back(j);
				else
					charsToRemove.clear();
			}
		}
	}

	for (int s = charsToRemove.size() - 1; s >= 0; s--)
	{
		str = str.remove(charsToRemove[s], 1);
	}

	if (str[str.count() - 1] == '.')
		str = str.remove(str.count() - 1, 1);

	return str;
}

void OutputWindow::clearOutput()
{
	output.clear();
	listModel->setStringList(output);
}

OutputWindow::~OutputWindow()
{
	delete listModel;
}