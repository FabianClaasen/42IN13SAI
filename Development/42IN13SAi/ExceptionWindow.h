#pragma once

#include <qlistview.h>
#include <qstringlist.h>
#include <qstandarditemmodel.h>
#include <qstringlistmodel.h>

class ExceptionWindow : public QListView
{
public:
	ExceptionWindow(QWidget *parent = 0);
	~ExceptionWindow();

	void addException(std::string exception);
	void clearAll();

private:
	QStringList exceptions;
	QStringListModel* listModel;
};