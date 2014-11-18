#pragma once

#include <QPlainTextEdit>
#include <QObject>
#include <qcompleter.h>
#include <memory>

class QListView;
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
		//virtual void focusInEvent(QFocusEvent *e);

	private slots:
		void updateLineNumberAreaWidth(int newBlockCount);
		void highlightCurrentLine();
		void updateLineNumberArea(const QRect &, int);

		// for codecompletion
		void insertCompletion(const QString &completion);
	private:
		QWidget *lineNumberArea;

		QListView *listView;
		QStringList words;
		QAbstractItemModel* modelFromFile(const QString& fileName);

		// for codecompletion
		QString textUnderCursor() const;
		QCompleter* compl;
		QTextEdit* textEdit;
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