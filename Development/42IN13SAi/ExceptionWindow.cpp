#include "ExceptionWindow.h"

ExceptionWindow::ExceptionWindow(QWidget *parent) : QListView(parent)
{
	this->setFont(QFont("Consolas", 9));
#ifndef _WIN32
	// Set font to bigger size for readability on Mac OS X
	this->setFont(QFont("Consolas", 12));
#endif

	setStyleSheet("color:red;");

	setMaximumHeight(100);
	setEditTriggers(QAbstractItemView::NoEditTriggers);
	setSelectionMode(QAbstractItemView::NoSelection);

	listModel = new QStringListModel(exceptions, nullptr);
	setModel(listModel);
}

void ExceptionWindow::addException(std::string exception)
{
	QString str = QString::fromUtf8(exception.c_str());
	exceptions << str;

	listModel->setStringList(exceptions);
}

void ExceptionWindow::clearExceptions()
{
	exceptions.clear();
	listModel->setStringList(exceptions);
}

ExceptionWindow::~ExceptionWindow()
{
}
