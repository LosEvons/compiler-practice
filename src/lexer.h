#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <optional>
#include "definitions.h"

#define TOKEN_HAS_VALUE		false
#define TOKEN_NULL_VALUE	true

enum SyntaxKind{
	BAD_TOKEN,
	EOF_TOKEN,
	WHITESPACE_TOKEN,
	NUMBER_TOKEN,
	PLUS_TOKEN,
	MINUS_TOKEN,
	ASTERISK_TOKEN,
	FSLASH_TOKEN,
	OPAR_TOKEN,
	CPAR_TOKEN,
	NUMBER_EXPRESSION_TOKEN,
	BINARY_EXPRESSION_TOKEN,
};

std::string GetSyntaxKindStr(SyntaxKind kind);

class SyntaxNode{
public:
	virtual SyntaxKind Kind(){ return BAD_TOKEN; };
	inline virtual std::vector<SyntaxNode*> GetChildren(){ std::vector<SyntaxNode*> childs; return childs; };
	inline virtual void PrintClassContents(){
		std::cout << ANSI_COLOR_BLUE << GetSyntaxKindStr(Kind()) << ANSI_COLOR_RESET << std::endl;
	};
};

class SyntaxToken : public SyntaxNode{
public:
	SyntaxKind Kind;
	int Position;
	std::string Text;
	int Value;
	bool Nullval;

	SyntaxToken(SyntaxKind kind, int position, std::string text, bool nullval = TOKEN_NULL_VALUE, int value = 0);
	void PrintClassContents() override{
		if (Nullval){
			std::cout << ANSI_COLOR_RED << GetSyntaxKindStr(Kind)
			<< ANSI_COLOR_YELLOW << " : " << Text << ANSI_COLOR_RESET << std::endl;
		}
		else{
			std::cout << ANSI_COLOR_RED << GetSyntaxKindStr(Kind)
			<< ANSI_COLOR_YELLOW << " : " << Text << " : " << Value << ANSI_COLOR_RESET << std::endl;
		}
	}
};

class ExpressionSyntax : public SyntaxNode{};

class NumberExpressionSyntax : public ExpressionSyntax{
public:
	SyntaxToken NumberToken;
	SyntaxKind Kind() override { return NUMBER_EXPRESSION_TOKEN; };
	NumberExpressionSyntax(const SyntaxToken numberToken) : NumberToken(numberToken){};
	std::vector<SyntaxNode*> GetChildren() override { std::vector<SyntaxNode*> childs{&NumberToken}; return childs; };
};

class BinaryExpressionSyntax : public ExpressionSyntax{
public:
	SyntaxKind Kind() override { return BINARY_EXPRESSION_TOKEN; };
	ExpressionSyntax* Left;
	SyntaxToken* OperatorToken;
	ExpressionSyntax* Right;
	BinaryExpressionSyntax(ExpressionSyntax* left, SyntaxToken* operatorToken, ExpressionSyntax* right);
	std::vector<SyntaxNode*> GetChildren() override { std::vector<SyntaxNode*> childs{Left, OperatorToken, Right}; return childs; };
};

class Lexer{
public:
	Lexer(std::string text);
	SyntaxToken* NextToken();
	std::vector<std::string> Diagnostics(){ return _diagnostics; };

private:
	std::string _text;
	int _position = 0;
	std::vector<std::string> _diagnostics = std::vector<std::string>();

	void Next();
	char Current();
};