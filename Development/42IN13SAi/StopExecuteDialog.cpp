#include "StopExecuteDialog.h"


StopExecuteDialog::StopExecuteDialog(QWidget* parent) : QDialog(parent)
{
	// Create the widgets
	dialogText = new QLabel("Er is nog een uitvoering bezig,\n wilt u de huidige uitvoering stoppen?");

	cancelButton = new QPushButton(tr("Cancel"));
	cancelButton->setDefault(true);

	okButton = new QPushButton(tr("OK"));
	okButton->setDefault(true);

	// Create the layouts
	QHBoxLayout* buttonLayout = new QHBoxLayout;
	buttonLayout->addWidget(cancelButton);
	buttonLayout->addWidget(okButton);

	QVBoxLayout* containerLayout = new QVBoxLayout;
	containerLayout->addWidget(dialogText);
	containerLayout->addLayout(buttonLayout);

	// Set the dialog views
	setLayout(containerLayout);
	setWindowTitle(tr("Stop execute"));
}


StopExecuteDialog::~StopExecuteDialog()
{
}

QPushButton* StopExecuteDialog::GetCancelButton()
{
	return cancelButton;
}

QPushButton* StopExecuteDialog::GetOkButton()
{
	return okButton;
}