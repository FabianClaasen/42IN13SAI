#include "CodeEditor.h"
#include <QPainter>
#include <QTextBlock>
#include <QCompleter>
#include <qabstractitemview.h>
#include <qapplication.h>
#include <qstringlistmodel.h>
#include <qscrollbar.h>

CodeEditor::CodeEditor(QWidget* parent) : QPlainTextEdit(parent), compl(0)
{
	this->setFont(QFont("Consolas", 9));
	QPalette pallete = this->palette();

	pallete.setColor(QPalette::Active, QPalette::Base, QColor(253, 246, 227));
	pallete.setColor(QPalette::Inactive, QPalette::Base, QColor(253, 246, 227));
	pallete.setColor(QPalette::Text, QColor(101, 123, 131));
	pallete.setColor(QPalette::Highlight, QColor(225, 219, 200));
	pallete.setColor(QPalette::HighlightedText, QColor(101, 123, 131)); 

	this->setPalette(pallete);
#ifndef _WIN32
	// Set font to bigger size for readability on Mac OS X
	this->setFont(QFont("Consolas", 12));
#endif

	this->setTabStopWidth(20);

	lineNumberArea = new LineNumberArea(this);

	connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
	connect(this, SIGNAL(updateRequest(QRect, int)), this, SLOT(updateLineNumberArea(QRect, int)));
	connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

	updateLineNumberAreaWidth(0);
	highlightCurrentLine();
}

int CodeEditor::lineNumberAreaWidth()
{
	int digits = 1;
	int max = qMax(1, blockCount());

	while (max >= 10)
	{
		max /= 10;
		++digits;
	}

	int width = fontMetrics().width(QLatin1Char('9')) * digits;

	return width + 10;
}

void CodeEditor::updateLineNumberAreaWidth(int)
{
	setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
{
	if (dy)
	{
		lineNumberArea->scroll(0, dy);
	}
	else
	{
		lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());
	}

	if (rect.contains(viewport()->rect()))
	{
		updateLineNumberAreaWidth(0);
	}
}

void CodeEditor::resizeEvent(QResizeEvent *e)
{
	QPlainTextEdit::resizeEvent(e);

	QRect cr = contentsRect();

	lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void CodeEditor::highlightCurrentLine()
{
	QList<QTextEdit::ExtraSelection> extraSelections;

	if (!isReadOnly())
	{
		QTextEdit::ExtraSelection selection;

		QColor lineColor = QColor(238, 232, 213);

		selection.format.setBackground(lineColor);
		selection.format.setProperty(QTextFormat::FullWidthSelection, true);
		selection.cursor = textCursor();
		selection.cursor.clearSelection();
		extraSelections.append(selection);
	}

	setExtraSelections(extraSelections);
}

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
	
	QPainter painter(lineNumberArea);
	painter.fillRect(event->rect(), Qt::lightGray);

	QTextBlock block = firstVisibleBlock();

	int blockNumber = block.blockNumber();
	int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
	int bottom = top + (int) blockBoundingRect(block).height();

	while (block.isValid() && top <= event->rect().bottom())
	{
		if (block.isVisible() && bottom >= event->rect().top())
		{
			QString number = QString::number(blockNumber + 1);
			painter.setPen(Qt::black);

			painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(), Qt::AlignRight, number);
		}

		block = block.next();
		top = bottom;
		bottom = top + (int) blockBoundingRect(block).height();

		++blockNumber;
	}
}

#pragma region codeCompletion

void CodeEditor::keyPressEvent(QKeyEvent *e)
{
	int key = e->key();
	bool isEnterAndListVisible;
	bool setIndexAfterPrefix = false;
	//std::cout << e->key() << std::endl;
	int row = 0;
	if (compl->popup()->isVisible())
	{
		// if enter, backspace or space key are pressed when popup is visible, hide it
		switch (key)
		{
		case 16777220: // enter key
		case 16777221: // tab key
		case 32: // space key
			compl->popup()->hide();
			return;
		}
		// If tab key is pressed, fill in the completion
		if ((Qt::Key_Tab == key) && compl->popup()->isEnabled())
		{
			QModelIndex currentIndex = compl->popup()->currentIndex();
			QString text = currentIndex.data().toString();
			isEnterAndListVisible = (key == Qt::Key_Tab) && compl->popup()->isVisible();

			if (!isEnterAndListVisible)
				QPlainTextEdit::keyPressEvent(e);

			compl->popup()->hide();
			insertCompletion(text);

			return;
		}
		// for runtime behaviour
		else
		{
			setCompletionPrefix("");
			setIndexAfterPrefix = true;
		}
	}

	// Shift behaviour as normal shift
	bool isShiftEnter = ((e->modifiers() & Qt::ShiftModifier) && e->key() == Qt::Key_Return);
	if (isShiftEnter)
		e = new QKeyEvent(e->type(), e->key(), e->modifiers()&Qt::MetaModifier&Qt::KeypadModifier, e->text(), e->isAutoRepeat(), (ushort)e->count());

	// Check if ctrl space is clicked
	bool isCtrlSpace = ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_Space);
	if (!isCtrlSpace)
		QPlainTextEdit::keyPressEvent(e);

	QString completionPrefix = textUnderCursor();

	// this one is for first the completerview and then fill in the text (runtime filling)
	if (setIndexAfterPrefix)
	{
		int count = compl->model()->rowCount();
		for (int i = 0; i < count; i++)
		{
			QString key = compl->completionModel()->index(i, 0).data().toString();
			if (key.startsWith(completionPrefix, Qt::CaseSensitive))
			{
				QModelIndex new_index = compl->completionModel()->index(i, 0);
				compl->popup()->setCurrentIndex(new_index);
				setCursorText(completionPrefix);
				compl->popup()->hide();
				setCompletionPrefix(completionPrefix);
				cr = getCompleterView();
				compl->complete(cr);
				return;
			}
		}
		// hide when case sensitive startswith is not found
		compl->popup()->hide();
	}
	
	setCompletionPrefix(completionPrefix);
	setCursorText(completionPrefix);
	cr = getCompleterView();
	if ((e->modifiers() & Qt::ControlModifier) && (e->key() == Qt::Key_Space))
		compl->complete(cr);
}

QAbstractItemModel* CodeEditor::modelFromFile(const QString& fileName)
{
	QFile file(fileName);
	if (!file.open(QFile::ReadOnly))
		return new QStringListModel(compl);

#ifndef QT_NO_CURSOR
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
#endif
	QStringList words;

	while (!file.atEnd()) {
		QByteArray line = file.readLine();
		if (!line.isEmpty())
			words << line.trimmed();
	}

#ifndef QT_NO_CURSOR
	QApplication::restoreOverrideCursor();
#endif
	return new QStringListModel(words, compl);
}

void CodeEditor::setCursorText(QString text)
{
	cursorText = text;
}

QString CodeEditor::getCursorText()
{
	return cursorText;
}

void CodeEditor::setCompleter(QCompleter *completer)
{
	if (compl)
		QObject::disconnect(completer, 0, this, 0);

	compl = completer;

	if (!compl)
		return;

	compl->setWidget(this);
	compl->setCompletionMode(QCompleter::PopupCompletion);
	compl->setCaseSensitivity(Qt::CaseSensitive);

	QObject::connect(compl, SIGNAL(activated(QString)),
		this, SLOT(insertCompletion(QString)));
}

void CodeEditor::setCompletionPrefix(QString completionPrefix)
{
	if (completionPrefix != compl->completionPrefix())
	{
		compl->setCompletionPrefix(completionPrefix);
		compl->popup()->setCurrentIndex(compl->completionModel()->index(0, 0));
	}
}

QRect CodeEditor::getCompleterView()
{
	QRect cr = cursorRect();
	cr.setX(20);
	cr.setWidth(compl->popup()->sizeHintForColumn(0)
		+ compl->popup()->verticalScrollBar()->sizeHint().width());

	return cr;
}

void CodeEditor::insertCompletion(const QString &text)
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

QString CodeEditor::textUnderCursor() const
{
	QTextCursor tc = textCursor();
	tc.select(QTextCursor::WordUnderCursor);
	return tc.selectedText();
}

QCompleter *CodeEditor::getCompleter() const
{
	return compl;
}

#pragma endregion codeCompletion
