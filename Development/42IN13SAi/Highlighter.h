#pragma once
#include "QSyntaxHighlighter.h"

class Highlighter : public QSyntaxHighlighter
{
	Q_OBJECT

public:
	Highlighter(QTextDocument *parent = 0);
	virtual ~Highlighter();

protected:
	void highlightBlock(const QString &text);

private:
	struct HighlightingRule
	{
		QRegExp pattern;
		QTextCharFormat format;
	};

	QVector<HighlightingRule> highlightingRules;

	QRegExp commentStartExpression;
	QRegExp commentEndExpression;

	// TODO OWN FORMATS FOR HIGHLIGHTING
	QTextCharFormat varFormat;
	QTextCharFormat funcFormat;
	QTextCharFormat returnFormat;
};

