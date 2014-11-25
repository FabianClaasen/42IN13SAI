#pragma once
#include <memory>
#include <iostream>

#include <QPlainTextEdit>
#include <QObject>
#include <qdir.h>
#include <QPainter>
#include <QTextBlock>
#include <qabstractitemview.h>
#include <qapplication.h>
#include <qstringlistmodel.h>
#include <qscrollbar.h>
#include <qabstractitemmodel.h>

class QCompleter;
class CodeEditor : public QPlainTextEdit
{
	Q_OBJECT

public:
		CodeEditor(QWidget* parent = 0);

		virtual void lineNumberAreaPaintEvent(QPaintEvent *event);
		int lineNumberAreaWidth();

		// for codecompletion
		void setCompleter(QCompleter *compl);
		QCompleter *getCompleter() const;

	protected:
		void resizeEvent(QResizeEvent *event);

		// for codecompletion
		virtual void keyPressEvent(QKeyEvent* e);
		virtual void focusInEvent(QFocusEvent *e);

	private slots:
		void updateLineNumberAreaWidth(int newBlockCount);
		void highlightCurrentLine();
		void updateLineNumberArea(const QRect &, int);

		// for codecompletion
		void insertCompletion(const QString &completion);

	private:
		QWidget *lineNumberArea;

		// for codecompletion
		QString textUnderCursor() const;
		QCompleter* compl;
		QTextEdit* textEdit;

		void checkBracketCharacter(QKeyEvent *e);
		void checkRightParenthesis();
		void setCompletionPrefix(QString text);

		QRect getCompleterView();
		QRect cr;
		QAbstractItemModel* modelFromFile(const QString& fileName);

		void addBrackets(QTextCursor tmpCursor, int pos, QString text, QString last, int spaces);
		QString completeCloseParentesis();
};

class LineNumberArea : public QWidget
{
	public:
		LineNumberArea(CodeEditor *editor) : QWidget(editor)
		{
			codeEditor = editor;
		}

		QSize sizeHint() const
		{
			return QSize(codeEditor->lineNumberAreaWidth(), 0);
		}

	protected:
		virtual void paintEvent(QPaintEvent *event)
		{
			codeEditor->lineNumberAreaPaintEvent(event);
		}

	private:
		CodeEditor *codeEditor;
};