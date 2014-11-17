#include "CodeCompleter.h"
#include <QKeyEvent>
#include <QCompleter>
#include <iostream>
#include "CodeEditor.h"
CodeCompleter::CodeCompleter(QWidget *parent) : CodeEditor(parent), compl(0)
{
	setPlainText(tr("This TextEdit provides autocompletions for words that have more than"
		" 3 characters. You can trigger autocompletion using ") +
		QKeySequence("Ctrl+Space").toString(QKeySequence::NativeText));

	setFocusPolicy(Qt::StrongFocus);
	setFocus(Qt::PopupFocusReason);
	setEnabled(true);
}

void CodeCompleter::setCompleter(QCompleter *completer)
{
	if (compl)
		QObject::disconnect(completer, 0, this, 0);

	compl = completer;

	if (!compl)
		return;

	compl->setWidget(this);
	compl->setCompletionMode(QCompleter::PopupCompletion);
	compl->setCaseSensitivity(Qt::CaseInsensitive);
	QObject::connect(compl, SIGNAL(activated(QString)),
		this, SLOT(insertCompletion(QString)));
}

QCompleter *CodeCompleter::completer() const
{
	return compl;
}

void CodeCompleter::insertCompletion(const QString &text)
{
	if (compl->widget() != this)
		return;

	QTextCursor tc = textCursor();
	int extra = text.length() - compl->completionPrefix().length();
	tc.movePosition(QTextCursor::Left);
	tc.movePosition(QTextCursor::EndOfWord);
	tc.insertText(text.right(extra));
	setTextCursor(tc);
}

QString CodeCompleter::textUnderCursor() const
{
	QTextCursor tc = textCursor();
	tc.select(QTextCursor::WordUnderCursor);
	return tc.selectedText();
}

void CodeCompleter::focusInEvent(QFocusEvent *e)
{
	if (compl)
		compl->setWidget(this);

	focusInEvent(e);
}

//void CodeCompleter::keyPressEvent(QKeyEvent *event)
//{
//	std::cout << "you printed " << event->key() << std::endl;
//	if (compl && compl->popup()->isVisible) {
//		// The following keys are forwarded by the completer to the widget
//		switch (keyevent->key()) {
//		case Qt::Key_Enter:
//		case Qt::Key_Return:
//		case Qt::Key_Escape:
//		case Qt::Key_Tab:
//		case Qt::Key_Backtab:
//			keyevent->ignore();
//			return; // let the completer do default behavior
//		default:
//			break;
//		}
//	}
//
//	bool isShortcut = ((keyevent->modifiers() & Qt::ControlModifier) && keyevent->key() == Qt::Key_Space); // CTRL+E
//	if (!compl || !isShortcut) // do not process the shortcut when we have a completer
//		QTextEdit::keyPressEvent(keyevent);
//
//	const bool ctrlOrShift = keyevent->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier);
//	if (!compl || (ctrlOrShift && keyevent->text().isEmpty()))
//		return;
//
//	static QString eow("~!@#$%^&*()_+{}|:\"<>?,./;'[]\\-="); // end of word
//	bool hasModifier = (keyevent->modifiers() != Qt::NoModifier) && !ctrlOrShift;
//	QString completionPrefix = textUnderCursor();
//
//	if (!isShortcut && (hasModifier || keyevent->text().isEmpty() || completionPrefix.length() < 3
//		|| eow.contains(keyevent->text().right(1)))) {
//		compl->popup()->hide();
//		return;
//	}
//
//	if (completionPrefix != compl->completionPrefix()) {
//		compl->setCompletionPrefix(completionPrefix);
//		compl->popup->setCurrentIndex(compl->completionModel->index(0, 0));
//	}
//	QRect cr = cursorRect();
//	cr.setWidth(compl->popup->sizeHintForColumn(0)+ compl->popup->verticalScrollBar->sizeHint.width);
//	compl->complete(cr); // popup it up!
//}

CodeCompleter::~CodeCompleter()
{
}
