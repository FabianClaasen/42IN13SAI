#include "Highlighter.h"
#include "Grammar.h"
#include "TokenDefinition.h"
#include <iostream>
#include "MyTokenType.h"
#include <iterator>
#include <qvector.h>
Highlighter::Highlighter(QTextDocument *parent) : QSyntaxHighlighter(parent)
{
	// get all definitions and iterate through that
	std::list<TokenDefinition> definitions = Grammar::getGrammar();
	std::list<TokenDefinition>::iterator iter;
	
	for (iter = definitions.begin(); iter != definitions.end(); iter++)
	{
		// Check which tokentype it is and set current format
		switch (iter->myTokenType)
		{
			case MyTokenType::If:
			case MyTokenType::Else:
			case MyTokenType::ElseIf:
			case MyTokenType::While:
			case MyTokenType::ForLoop:
			case MyTokenType::Var:
				setCurrentFormat(currentFormat, rule, QRegExp("\\bif|else|while|frl|var"), QFont::Bold, QColor(153, 0, 153)); // sort of purple
				break;
			case MyTokenType::Return:
			case MyTokenType::Void:
			case MyTokenType::Float:
				setCurrentFormat(currentFormat, rule, QRegExp("\\bvoid|float|ret\\b"), QFont::Bold, Qt::darkBlue);
				break;
			case MyTokenType::Function:
			case MyTokenType::MainFunction:
				setCurrentFormat(currentFormat, rule, QRegExp("\\b(?!if|else|while|frl)[A-Za-z0-9_]+(?=\\()"), QFont::Normal, Qt::blue);
				break;
			default:
				break;
		}
	}

	commentStartExpression = QRegExp("/\\*");
	commentEndExpression = QRegExp("\\*/");
}

void Highlighter::setCurrentFormat(QTextCharFormat currentFormat, HighlightingRule rule, QRegExp regexPattern, QFont::Weight font, QColor color)
{
	currentFormat.setFontWeight(font);
	currentFormat.setForeground(color);
	rule.format = currentFormat;
	rule.pattern = regexPattern;
	highlightingRules.append(rule);
}

void Highlighter::highlightBlock(const QString &text)
{
	foreach(const HighlightingRule &rule, highlightingRules) 
	{
		QRegExp expression(rule.pattern);
		int index = expression.indexIn(text);
		while (index >= 0) 
		{
			int length = expression.matchedLength();
			setFormat(index, length, rule.format);
			index = expression.indexIn(text, index + length);
		}
	}
}

Highlighter::~Highlighter()
{
}
