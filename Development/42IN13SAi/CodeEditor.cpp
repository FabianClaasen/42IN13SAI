#include "CodeEditor.h"
#include <QCompleter>

CodeEditor::CodeEditor(QWidget* parent) : QPlainTextEdit(parent), completer(0)
{
	QString resourceDir = QDir::currentPath().append("/Resources/");
#ifndef _WIN32
    resourceDir = QCoreApplication::applicationDirPath() + "/";
#endif
    
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

	//setFocus(Qt::OtherFocusReason);
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

	int fontWidth = fontMetrics().width(QLatin1Char('9'));
	int width = 0;

	(digits < 5) ? width = fontWidth * 4 : width = fontWidth * digits;

	return width;
}

void CodeEditor::updateLineNumberAreaWidth(int)
{
	setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
{
	(dy) ? lineNumberArea->scroll(0, dy) : lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

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

int CodeEditor::lineNumberAtPos(int pos)
{
	return toPlainText().left(pos).count("\n");
}

QString CodeEditor::getLine(int lineNumber) const
{
	QTextCursor curs(document());

	if (lineNumber > 0)
	{
		if (curs.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, lineNumber) == false)
		{
			return QString::null;
		}

		curs.movePosition(QTextCursor::StartOfLine);
	}

	curs.anchor();
	curs.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);

	return curs.selectedText();
}

bool CodeEditor::replaceLine(int lineNumber, const QString& str)
{
	QTextCursor cursor(document());

	bool replaced = true;
	bool inPlace = true;

	if (lineNumber > 0)
	{
		if (cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, lineNumber) == false)
		{
			inPlace = false;
		}
		else
		{
			cursor.movePosition(QTextCursor::StartOfLine);
		}
	}

	if (inPlace)
	{
		cursor.anchor();

		cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
		cursor.removeSelectedText();
		cursor.insertText(str);

		replaced = true;
	}

	return replaced;
}

void CodeEditor::keyPressEvent(QKeyEvent *e)
{
	int key = e->key();
	// For a better behaviour of completer and enter
	bool isEnterAndListVisible;
	bool setIndexAfterPrefix = false;
	// Check if completer is visible
	if (completer->popup()->isVisible())
	{
		// if space or escape is pressed when popup is visible, hide it
		switch (key)
		{
		case Qt::Key_Space:
		case Qt::Key_Escape:
			completer->popup()->hide();
			return;
		}
		// If tab key or enter is pressed, fill in the completion and return
		if ((Qt::Key_Tab == key || key == Qt::Key_Return) && completer->popup()->isEnabled())
		{
			QModelIndex currentIndex = completer->popup()->currentIndex();
			QString text = currentIndex.data().toString();
			isEnterAndListVisible = (key == Qt::Key_Tab || key == Qt::Key_Return) && completer->popup()->isVisible();

			if (!isEnterAndListVisible)
				QPlainTextEdit::keyPressEvent(e);

			completer->popup()->hide();
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
		QTextCursor curs = textCursor();

		if (curs.hasSelection())
		{
			e->ignore();
			e->setAccepted(true);

			if (key == Qt::Key_Tab && !e->modifiers())
			{
				int startPos = curs.selectionStart();
				int endPos = curs.selectionEnd();

				curs.setPosition(startPos);
				curs.movePosition(QTextCursor::StartOfLine);

				while (curs.position() < endPos && curs.position() >= 0)
				{
					curs.insertText("\t");

					curs.movePosition(QTextCursor::EndOfLine);
					curs.movePosition(QTextCursor::NextCharacter);

					endPos++;
				}

				return;
			}
			else if (key == Qt::Key_Backtab)
			{
				int startPos = curs.selectionStart();
				int endPos = curs.selectionEnd();

				int lineStart = lineNumberAtPos(startPos);
				int lineEnd = lineNumberAtPos(endPos);

				for (int i = lineStart; i <= lineEnd; ++i)
				{
					QString line = getLine(i);

					int length = std::min(1, line.length());
					int tabs;

					for (tabs = 0; tabs < length; ++tabs)
					{
						if (line[tabs] != '\t') break;
					}

					if (tabs > 0)
					{
						line.remove(0, tabs);
						replaceLine(i, line);
					}
				}

				return;
			}
		}

		// Check for special indents
		if (key == Qt::Key_Tab || key == Qt::Key_Return)
		{
			QTextCursor tmpCursor = textCursor();
			if (tmpCursor.positionInBlock() > 0)
			{
				QString line = getSentenceFromLine();
				if (key == Qt::Key_Tab && line.contains("(") && line.contains(")") && line.contains("func"))
				{
					tmpCursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, 1);
					setTextCursor(tmpCursor);
					insertPlainText("[");
					addSpecialIndent(false, true);
					return;
				}	
				else if (key == Qt::Key_Return)
				{
					addSpecialIndent(true, false);
					return;
				}
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

	// get completion prefix
	QString completionPrefix = textUnderCursor();
	if (completionPrefix.contains("(") || completionPrefix.contains(")"))
		completionPrefix = "";
	
	checkBracketCharacter(e);

	setCompletionPrefix(completionPrefix);
	cr = getCompleterView();
	// Check for automatic completion or not
	if ((e->modifiers() & Qt::ControlModifier) && (e->key() == Qt::Key_Space))
	{
		QModelIndex index = completer->popup()->currentIndex();
		int count = completer->completionCount();
		if (count == 1)
		{
			QString text = index.data().toString();
			insertCompletion(text);
		}
		else
			completer->complete(cr);
	}
	else if (setIndexAfterPrefix)
	{
		int count = completer->model()->rowCount();
		for (int i = 0; i < count; i++)
		{
			QString key = completer->completionModel()->index(i, 0).data().toString();
			if (key.startsWith(completionPrefix, Qt::CaseSensitive))
			{
				QModelIndex new_index = completer->completionModel()->index(i, 0);
				completer->popup()->setCurrentIndex(new_index);
				setCompletionPrefix(completionPrefix);
				cr = getCompleterView();
				completer->complete(cr);
				return;
			}
		}
		// hide when case sensitive startswith is not found
		completer->popup()->hide();
	}
}

QAbstractItemModel* CodeEditor::modelFromFile(const QString& fileName)
{
	QFile file(fileName);
	if (!file.open(QFile::ReadOnly))
		return new QStringListModel(completer);

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
	return new QStringListModel(words, completer);
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
	else
		insertPlainText(")");
}

QString CodeEditor::getSentenceFromLine()
{
	QTextCursor tmpCursor = textCursor();
	int pos = textCursor().block().firstLineNumber();
	QString text = document()->findBlockByLineNumber(pos).text();
	return text;
}

void CodeEditor::addSpecialIndent(bool isEnter, bool isBracket)
{
	QTextCursor tmpCursor = textCursor();
	int pos = textCursor().block().firstLineNumber();
	int spaces = 0;
	
	QString last;
	QString text = document()->findBlockByLineNumber(pos).text();

	if(isBracket) text.remove(text.count() - 1, 1);
	(text.size() == 0) ? last = "" : last = text[text.length() - 1];

	if (isEnter) insertPlainText("\n");

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
		if (isBracket)
		{
			textCursor().deletePreviousChar();
			insertPlainText("\n");
		}

		for (int i = 0; i < text.count(); i++)
		{
			if (text[i] == ' ')
			{
				spaces++;
				if(isBracket) insertPlainText(" ");
			}
			else if (text[i] == '\t')
			{
				spaces = spaces + 3;
				if(isBracket) insertPlainText("\t");
			}
			else
				break;
		}
		if (isBracket) insertPlainText("[");
	}

	if (isBracket)
	{
		insertPlainText("\n\n");
		for (int j = 0; j < spaces; j++)
			insertPlainText(" ");
		insertPlainText("]");

		tmpCursor = textCursor();
		tmpCursor.movePosition(QTextCursor::Up, QTextCursor::MoveAnchor, 1);
		setTextCursor(tmpCursor);
	}

	if (spaces > 0)
	{
		for (int s = 0; s < spaces; s++)
			insertPlainText(" ");
		
	}
	if(isBracket) insertPlainText("\t");
}

void CodeEditor::checkBracketCharacter(QKeyEvent *e)
{
	if (e->key() == Qt::Key_BracketLeft || Qt::Key_ParenLeft)
	{
		switch (e->key())
		{
			case Qt::Key_BracketLeft:
				//addBrackets(tmpCursor, pos, text, last, spaces);
				addSpecialIndent(false, true);
				break;
			case Qt::Key_ParenLeft:
				insertPlainText(")");
				QTextCursor tmpCursor = textCursor();
				tmpCursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, 1);
				setTextCursor(tmpCursor);
				break;
		}
	}
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

void CodeEditor::setCompleter(QCompleter *completerNew)
{
	if (completer)
		QObject::disconnect(completerNew, 0, this, 0);

	completer = completerNew;

	if (!completer)
		return;

	completer->setWidget(this);
	completer->setCompletionMode(QCompleter::PopupCompletion);
	completer->setCaseSensitivity(Qt::CaseSensitive);

	QObject::connect(completer, SIGNAL(activated(QString)),
		this, SLOT(insertCompletion(QString)));
}

void CodeEditor::setCompletionPrefix(QString completionPrefix)
{
	if (completionPrefix != completer->completionPrefix())
	{
		completer->setCompletionPrefix(completionPrefix);
		completer->popup()->setCurrentIndex(completer->completionModel()->index(0, 0));
	}
}

QRect CodeEditor::getCompleterView()
{
	QMargins margin = QMargins(-19, 0, 0, 0);
	QRect cr = cursorRect().marginsAdded(margin);
	cr.setWidth(completer->popup()->sizeHintForColumn(0)
		+ completer->popup()->verticalScrollBar()->sizeHint().width());

	return cr;
}

void CodeEditor::insertCompletion(const QString &text)
{
	if (completer->widget() != this)
		return;

	QTextCursor tc = textCursor();
	int extra = text.length() - completer->completionPrefix().length();
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
	return completer;
}

void CodeEditor::focusInEvent(QFocusEvent *e)
{
	if (completer)
		completer->setWidget(this);

	QPlainTextEdit::focusInEvent(e);
}

#pragma endregion codeCompletion
