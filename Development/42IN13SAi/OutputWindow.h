#pragma once

#include <cfloat>
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
	QString setOutputPrecision(QString str);
	void clearOutput();
    
	void SetTheme(std::map<std::string, QColor> colors);
private:
	QStringList output;
	QStringListModel* listModel;
};