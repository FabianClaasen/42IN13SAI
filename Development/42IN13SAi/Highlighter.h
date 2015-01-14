#pragma once
#include <QSyntaxHighlighter>

class Highlighter : public QSyntaxHighlighter
{
	Q_OBJECT

public:
	Highlighter(std::map<std::string, QColor> colors, QTextDocument *parent = 0);
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

	QTextCharFormat currentFormat;
	HighlightingRule rule;

	void setCurrentFormat(QTextCharFormat format, HighlightingRule rule, QRegExp regexPattern, QFont::Weight font, QColor color);
};

