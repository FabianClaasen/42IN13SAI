#pragma once

#include <qlistview.h>
#include <qstringlist.h>
#include <qstandarditemmodel.h>
#include <qstringlistmodel.h>
#include <QAbstractItemView>

class ExceptionWindow : public QListView
{
	Q_OBJECT

public:
	ExceptionWindow(QWidget *parent = 0);
	~ExceptionWindow();

	void addException(std::string exception);
	void clearExceptions();

	void SetTheme(std::map<std::string, QColor> colors);
	
private:
	QStringList exceptions;
	QStringListModel* listModel;
};