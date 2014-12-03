#include "OutputWindow.h"

OutputWindow::OutputWindow(QWidget *parent) : QListView(parent)
{
	this->setFont(QFont("Consolas", 9));
#ifndef _WIN32
	// Set font to bigger size for readability on Mac OS X
	this->setFont(QFont("Consolas", 12));
#endif

	setEditTriggers(QAbstractItemView::NoEditTriggers);
	setSelectionMode(QAbstractItemView::NoSelection);

	listModel = new QStringListModel(output, nullptr);
	setModel(listModel);
}

void OutputWindow::addOutput(std::string strOutput)
{
	QString str = QString::fromUtf8(strOutput.c_str());
	// Algoritm for output precision
	str = setOutputPrecision(str);
	output << str;

	listModel->setStringList(output);
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
				if (str[j] == '0')
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