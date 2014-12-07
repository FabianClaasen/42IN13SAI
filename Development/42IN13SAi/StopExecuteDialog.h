#pragma once

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

class StopExecuteDialog : public QDialog
{
	Q_OBJECT

public:
	StopExecuteDialog(QWidget* parent = 0);
	~StopExecuteDialog();

	// Get functions
	QPushButton* GetCancelButton();
	QPushButton* GetOkButton();

private:
	QLabel* dialogText;
	QPushButton* cancelButton;
	QPushButton* okButton;
};