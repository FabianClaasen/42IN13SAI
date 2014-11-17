#pragma once
#include "CodeEditor.h"
class QCompleter;
class CodeCompleter : public CodeEditor
{
	Q_OBJECT

public:
	CodeCompleter(QWidget *parent = 0);
	virtual ~CodeCompleter();

	void setCompleter(QCompleter *compl);
	QCompleter *completer() const;
	
protected:
	//virtual void keyPressEvent(QKeyEvent* keyevent);
	void focusInEvent(QFocusEvent *e);

	private slots :
		void insertCompletion(const QString &completion);

private:
	QString textUnderCursor() const;
	QCompleter *compl;
	QTextEdit* textEdit;
};

