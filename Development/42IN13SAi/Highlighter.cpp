#include "Highlighter.h"


Highlighter::Highlighter(QTextDocument *parent) : QSyntaxHighlighter(parent)
{
	HighlightingRule rule;

	varFormat.setFontWeight(QFont::Bold);
	varFormat.setForeground(Qt::darkGreen);
	rule.pattern = QRegExp("\\bvar\\b"); // regex that contains var
	rule.format = varFormat;
	highlightingRules.append(rule);

	funcFormat.setForeground(Qt::blue);
	rule.pattern = QRegExp("\\b[A-Za-z0-9_]+(?=\\()"); // regex that contains a function with parenthesis
	rule.format = funcFormat;
	highlightingRules.append(rule);

	returnFormat.setFontWeight(QFont::Bold);
	returnFormat.setForeground(Qt::darkBlue);
	rule.pattern = QRegExp("\\bvoid|float\\b"); // regex that contains void or float
	rule.format = returnFormat;
	highlightingRules.append(rule);

	commentStartExpression = QRegExp("/\\*");
	commentEndExpression = QRegExp("\\*/");
}

void Highlighter::highlightBlock(const QString &text)
{
	foreach(const HighlightingRule &rule, highlightingRules) {
		QRegExp expression(rule.pattern);
		int index = expression.indexIn(text);
		while (index >= 0) {
			int length = expression.matchedLength();
			setFormat(index, length, rule.format);
			index = expression.indexIn(text, index + length);
		}
	}
}

Highlighter::~Highlighter()
{
}
