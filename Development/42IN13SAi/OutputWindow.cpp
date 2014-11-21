#include "OutputWindow.h"

OutputWindow::OutputWindow(QWidget *parent) : QListView(parent)
{
	this->setFont(QFont("Consolas", 9));
#ifndef _WIN32
	// Set font to bigger size for readability on Mac OS X
	this->setFont(QFont("Consolas", 12));
#endif

	setMaximumHeight(100);
	setEditTriggers(QAbstractItemView::NoEditTriggers);
	setSelectionMode(QAbstractItemView::NoSelection);

	listModel = new QStringListModel(output, nullptr);
	setModel(listModel);
}

void OutputWindow::addOutput(std::string exception)
{
	QString str = QString::fromUtf8(exception.c_str());
	output << str;

	listModel->setStringList(output);
}

void OutputWindow::clearOutput()
{
	output.clear();
	listModel->setStringList(output);
}

OutputWindow::~OutputWindow()
{
}