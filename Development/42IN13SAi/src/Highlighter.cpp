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
			case MyTokenType::Var:
			case MyTokenType::Void:
			case MyTokenType::FloatReturn:
				setCurrentFormat(currentFormat, rule, QRegExp(QString::fromUtf8(iter->matcher.GetRegexString().c_str())), QFont::Normal, QColor(203, 75, 22));
				break;
			case MyTokenType::If:
			case MyTokenType::Else:
			case MyTokenType::ElseIf:
			case MyTokenType::While:
			case MyTokenType::ForLoop:
				setCurrentFormat(currentFormat, rule, QRegExp(QString::fromUtf8(iter->matcher.GetRegexString().c_str())), QFont::Normal, QColor(133, 153, 0));
				break;
			case MyTokenType::Function:
			case MyTokenType::MainFunction:
				setCurrentFormat(currentFormat, rule, QRegExp("\\b(?!if|else|while|frl)[A-Za-z0-9_]+(?=\\()"), QFont::Bold, QColor(88, 110, 117));
				break;
			case MyTokenType::OperatorDivide:
			case MyTokenType::OperatorMinus:
			case MyTokenType::OperatorMultiply:
			case MyTokenType::OperatorPlus:
			case MyTokenType::OperatorRaised:
			case MyTokenType::UniOperatorMinus:
			case MyTokenType::UniOperatorPlus:
				setCurrentFormat(currentFormat, rule, QRegExp(QString::fromUtf8(iter->matcher.GetRegexString().c_str())), QFont::Normal, QColor(211, 54, 130));
				break;
			case MyTokenType::Float:
				setCurrentFormat(currentFormat, rule, QRegExp(QString::fromUtf8(iter->matcher.GetRegexString().c_str())), QFont::Normal, QColor(42, 161, 152));
				break;
			default:
				break;
		}
	}

	// Rule for func
	setCurrentFormat(currentFormat, rule, QRegExp("\\bfunc\\b"), QFont::Normal, QColor(203, 75, 22));

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
