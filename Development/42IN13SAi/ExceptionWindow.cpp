#include "ExceptionWindow.h"

ExceptionWindow::ExceptionWindow(QWidget *parent) : QListView(parent)
{
	this->setFont(QFont("Consolas", 9));
#ifndef _WIN32
	// Set font to bigger size for readability on Mac OS X
	this->setFont(QFont("Consolas", 12));
#endif

	setEditTriggers(QAbstractItemView::NoEditTriggers);
	setSelectionMode(QAbstractItemView::NoSelection);

	listModel = new QStringListModel(exceptions, nullptr);
	setModel(listModel);
}

void ExceptionWindow::SetTheme(std::map<std::string, QColor> colors)
{
	// Output colours
	QPalette pallete;
	pallete.setColor(QPalette::Active, QPalette::Base, colors["background"]);
	pallete.setColor(QPalette::Text, colors["text"]);

	this->setPalette(pallete);
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
	delete listModel;
}
