#pragma once
#include <iostream>
#include <string>
#include <list>
#include <ranges>
#include <vector>
#include <algorithm>
#include "lexer.h"

class Parser{
public:
	explicit Parser(std::string text);
	SyntaxTree* Parse();
	std::vector<std::string> Diagnostic() const { return _diagnostics; };
	
private:
	std::vector<SyntaxToken*> _tokens;
	int _position = 0;
	std::vector<std::string> _diagnostics;

	SyntaxToken* Peek(int offset);
	SyntaxToken* Current() { return Peek(0); };
	SyntaxToken* NextToken();
	SyntaxToken* Match(SyntaxKind kind);
	ExpressionSyntax* ParsePrimaryExpression();
	ExpressionSyntax* ParseExpression();
	ExpressionSyntax* ParseTerm();
	ExpressionSyntax* ParseFactor();
};

class Evaluator{
public:
	static int Evaluate(ExpressionSyntax* _root){ return EvaluateExpression(_root); };
	static int EvaluateExpression(ExpressionSyntax* root);
	
};