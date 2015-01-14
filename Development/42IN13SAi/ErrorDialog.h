#pragma once

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

class ErrorDialog : public QDialog
{
public:
	ErrorDialog(std::string error, QWidget* parent = 0);
	~ErrorDialog();

	QPushButton* GetCloseButton();

private:
	QLabel* dialogText;
	QPushButton* closeButton;
};

