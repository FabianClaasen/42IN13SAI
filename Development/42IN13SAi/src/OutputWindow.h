#pragma once

#include <qlistview.h>
#include <qstringlist.h>
#include <qstandarditemmodel.h>
#include <qstringlistmodel.h>
#include <QAbstractItemView>

class OutputWindow : public QListView
{
	Q_OBJECT
public:
	OutputWindow(QWidget *parent = 0);
	~OutputWindow();

	void addOutput(std::string strOutput);
	void clearOutput();

private:
	QStringList output;
	QStringListModel* listModel;
};