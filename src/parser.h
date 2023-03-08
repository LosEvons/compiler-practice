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
	Parser(std::string text);
	~Parser();
	SyntaxTree* Parse();
	std::vector<std::string> Diagnostic(){ return _diagnostics; };

private:
	std::vector<SyntaxToken*> _tokens;
	int _position = 0;
	std::vector<std::string> _diagnostics;;

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
	Evaluator(ExpressionSyntax* root) : _root(root){};
	//~Evaluator(){ delete _root; };
	int Evaluate(){ return EvaluateExpression(_root); };
	int EvaluateExpression(ExpressionSyntax* root);
private:
	ExpressionSyntax* _root;
	
};