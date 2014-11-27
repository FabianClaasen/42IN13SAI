#include "CodeEditor.h"
#include <QCompleter>

CodeEditor::CodeEditor(QWidget* parent) : QPlainTextEdit(parent), compl(0)
{
	QString resourceDir = QDir::currentPath().append("/Resources/");
	// Add default font to the font database
	QFontDatabase fontDatabase;
	fontDatabase.addApplicationFont(resourceDir + "DejaVuSansMono.ttf");
	fontDatabase.addApplicationFont(resourceDir + "DejaVuSansMono-Bold.ttf");
	fontDatabase.addApplicationFont(resourceDir + "DejaVuSansMono-Oblique.ttf");

	lineNumberArea = new LineNumberArea(this);

	connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
	connect(this, SIGNAL(updateRequest(QRect, int)), this, SLOT(updateLineNumberArea(QRect, int)));
	connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

	updateLineNumberAreaWidth(0);

	setFocus(Qt::OtherFocusReason);
}

void CodeEditor::SetTheme(std::map<std::string, QColor> colors, std::string fontFamily, int fontSize)
{
	// Set the font
	QFont ideFont = QFont(QString::fromStdString(fontFamily), fontSize);
	ideFont.setStyleStrategy(QFont::PreferAntialias);

	// Set editor font
	setFont(ideFont);
	QFontMetrics metrics(ideFont);

	// Set tabs
	const int tabStop = 3;
	setTabStopWidth(tabStop * metrics.width(' '));

	// Editor colours
	QPalette pallete;

	pallete.setColor(QPalette::Active, QPalette::Base, colors["background"]);
	pallete.setColor(QPalette::Inactive, QPalette::Base, colors["background"]);
	pallete.setColor(QPalette::Text, colors["text"]);
	pallete.setColor(QPalette::Highlight, colors["higlight"]);
	pallete.setColor(QPalette::HighlightedText, colors["highlighted_text"]);

	this->setPalette(pallete);

	// Line number area
	lineNumberBackground = colors["linenumber_background"];
	lineNumberText = colors["linenumber_text"];

	// Set the current line higlight
	currentLineBackground = colors["currentline_background"];
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
	setExtraSelections(extraSelections);

	if (!isReadOnly())
	{
		QTextEdit::ExtraSelection selection;

		QColor lineColor = currentLineBackground;

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
	painter.fillRect(event->rect(), lineNumberBackground);

	QTextBlock block = firstVisibleBlock();

	int blockNumber = block.blockNumber();
	int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
	int bottom = top + (int) blockBoundingRect(block).height();

	while (block.isValid() && top <= event->rect().bottom())
	{
		if (block.isVisible() && bottom >= event->rect().top())
		{
			QString number = QString::number(blockNumber + 1);
			painter.setPen(lineNumberText);

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
		// if backspace or space key are pressed when popup is visible, hide it
		switch (key)
		{
		case Qt::Key_Space:
		case Qt::Key_Backspace: 
		case Qt::Key_Escape:
			compl->popup()->hide();
			return;
		}
		// If tab key is pressed, fill in the completion
		if ((Qt::Key_Tab == key || key == Qt::Key_Return) && compl->popup()->isEnabled())
		{
			QModelIndex currentIndex = compl->popup()->currentIndex();
			QString text = currentIndex.data().toString();
			isEnterAndListVisible = (key == Qt::Key_Tab || key == Qt::Key_Return) && compl->popup()->isVisible();

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
	else 
	{
		if (textCursor().position() > 0)
		{			
			QString previousCharacter = toPlainText().at(textCursor().position() - 1);
			QString nextCharacter = completeCloseParentesis();
			QTextCursor tmpCursor = textCursor();
			if (key == Qt::Key_Tab && previousCharacter == "(" && nextCharacter == ")")
			{
				tmpCursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, 1);
				setTextCursor(tmpCursor);
				insertPlainText("[");
				addBrackets(tmpCursor, 0, "", "", 0);
				return;
			}
		}
	}

	if (key == Qt::Key_ParenRight)
	{
		checkRightParenthesis();
		return;
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
	if (completionPrefix.contains("(") || completionPrefix.contains(")"))
		completionPrefix = "";

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
	
	checkBracketCharacter(e);

	setCompletionPrefix(completionPrefix);
	cr = getCompleterView();
	if ((e->modifiers() & Qt::ControlModifier) && (e->key() == Qt::Key_Space))
	{
		QModelIndex index = compl->popup()->currentIndex();
		int count = compl->completionCount();
		if (count == 1)
		{
			QString text = index.data().toString();
			insertCompletion(text);
		}
		else
			compl->complete(cr);
	}
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

void CodeEditor::checkRightParenthesis()
{
	QString prevChar = toPlainText().at(textCursor().position() - 1);
	QTextCursor tc = textCursor();
	QString result = completeCloseParentesis();
	if (result == ")" && prevChar != ")")
	{
		tc.movePosition(QTextCursor::EndOfBlock);
		insertPlainText(")");
		tc.deletePreviousChar();
	}
	else if (prevChar == ")" || prevChar != "(")
	{
		insertPlainText(")");
	}
}
void CodeEditor::checkBracketCharacter(QKeyEvent *e)
{
	if (e->key() == Qt::Key_BracketLeft || Qt::Key_ParenLeft)
	{
		QTextCursor tmpCursor;
		QString text;
		QString insertNewValue;
		QTextBlock block;
		int pos = 0;
		int spaces = 0;
		QString last;
		switch (e->key())
		{
			case Qt::Key_BracketLeft:
				addBrackets(tmpCursor, pos, text, last, spaces);
				break;
			case Qt::Key_ParenLeft:
				insertPlainText(")");
				tmpCursor = textCursor();
				tmpCursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, 1);
				setTextCursor(tmpCursor);
				break;
		}
	}
}

void CodeEditor::addBrackets(QTextCursor tmpCursor, int pos, QString text, QString last, int spaces)
{
	pos = textCursor().block().firstLineNumber();

	text = document()->findBlockByLineNumber(pos).text();

	text.remove(text.count() - 1, 1);
	if (text.size() == 0)
	{
		last = " ";
	}
	else
	{
		last = text[text.length() - 1];
	}

	if (last.isEmpty() || last == " " || last == "\t")
	{

		for (int l = 0; l < text.count(); l++)
		{
			if (text[l] == ' ')
			{
				spaces++;
			}
			else if (text[l] == '\t')
				spaces = spaces + 3;
		}
	}
	else
	{
		textCursor().deletePreviousChar();
		insertPlainText("\n");
		for (int i = 0; i < text.count(); i++)
		{
			if (text[i] == ' ')
			{
				spaces++;
				insertPlainText(" ");
			}
			else if (text[i] == '\t')
			{
				spaces = spaces + 3;
				insertPlainText("   ");
			}
			else
				break;
		}
		insertPlainText("[");
	}


	insertPlainText("\n\n");
	for (int j = 0; j < spaces; j++)
		insertPlainText(" ");
	insertPlainText("]");

	tmpCursor = textCursor();
	tmpCursor.movePosition(QTextCursor::Up, QTextCursor::MoveAnchor, 1);
	setTextCursor(tmpCursor);
	if (spaces > 0)
	{
		for (int s = 0; s < spaces; s++)
			insertPlainText(" ");
	}
	insertPlainText("   ");
}

QString CodeEditor::completeCloseParentesis()
{
	int pos = textCursor().block().firstLineNumber();

	QString text = document()->findBlockByLineNumber(pos).text();
	QString result = "";
	if (text != "")
		result = text.at(text.count() - 1);

	return result;
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
	QMargins margin = QMargins(-19, 0, 0, 0);
	QRect cr = cursorRect().marginsAdded(margin);
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

void CodeEditor::focusInEvent(QFocusEvent *e)
{
	if (compl)
		compl->setWidget(this);

	QPlainTextEdit::focusInEvent(e);
}

#pragma endregion codeCompletion
