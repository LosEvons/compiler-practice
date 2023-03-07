#pragma once
#include <iostream>
#include <string>
#include <list>
#include <ranges>
#include <vector>
#include "lexer.h"

class Parser{
public:
	Parser(std::string text);
	ExpressionSyntax* Parse();

private:
	std::vector<SyntaxToken> _tokens;
	int _position;

	SyntaxToken* Peek(int offset);
	inline SyntaxToken* Current() { return Peek(0); };
	SyntaxToken* NextToken();
	SyntaxToken* Match(SyntaxKind kind);
	ExpressionSyntax* ParsePrimaryExpression();
};