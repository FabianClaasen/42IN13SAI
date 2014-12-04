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
    // parse the string
    float to_format = ::atof(str.toStdString().c_str());
    
    // Get the size needed for the float digits (copied from internal to_string calculation)
    const int max_digits = std::numeric_limits<float>::max_exponent10 + 20;
    char buffer[max_digits];

#ifndef _WIN32
    snprintf(buffer, sizeof(buffer), "%g", to_format);
#else
    _snprintf(buffer, sizeof(buffer), "%g", to_format);
#endif
	return QString::fromStdString(buffer);
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