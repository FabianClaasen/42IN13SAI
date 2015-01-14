#include "ErrorDialog.h"


ErrorDialog::ErrorDialog(std::string error, QWidget* parent) : QDialog(parent)
{
	// Create the widgets
	dialogText = new QLabel(QString::fromUtf8(error.c_str()));

	closeButton = new QPushButton(tr("Ok"));
	closeButton->setDefault(true);

	// Create the layouts
	QHBoxLayout* buttonLayout = new QHBoxLayout;
	buttonLayout->addWidget(closeButton);

	QVBoxLayout* containerLayout = new QVBoxLayout;
	containerLayout->addWidget(dialogText);
	containerLayout->addLayout(buttonLayout);

	// Set the dialog views
	setLayout(containerLayout);
	setWindowTitle(tr("Error"));
}

QPushButton* ErrorDialog::GetCloseButton()
{
	return closeButton;
}

ErrorDialog::~ErrorDialog()
{
}