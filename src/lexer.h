#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <optional>

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
	BINARY_EXPRESSION_TOKEN,
};

class SyntaxToken{
public:
	SyntaxKind Kind;
	int Position;
	std::string Text;
	int Value;
	bool Nullval;

	SyntaxToken(SyntaxKind kind, int position, std::string text, bool nullval = TOKEN_NULL_VALUE, int value = 0);
};

class SyntaxNode{
public:
	virtual SyntaxKind Kind() = 0;
	//virtual std::vector<SyntaxNode> Children() = 0;
};

class ExpressionSyntax : public SyntaxNode{};

class NumberExpressionSyntax : public ExpressionSyntax{
public:
	SyntaxKind Kind() override { return NUMBER_TOKEN; };
	SyntaxToken NumberToken;
	
	NumberExpressionSyntax(const SyntaxToken numberToken) : NumberToken(numberToken){};	
};

class BinaryExpressionSyntax : public ExpressionSyntax{
public:
	SyntaxKind Kind() override { return BINARY_EXPRESSION_TOKEN; };

	ExpressionSyntax* Left;
	SyntaxToken* OperatorToken;
	ExpressionSyntax* Right;
	BinaryExpressionSyntax(ExpressionSyntax* left, SyntaxToken* operatorToken, ExpressionSyntax* right);
};

class Lexer{
public:
	Lexer(std::string text);

	SyntaxToken* NextToken();

private:
	std::string _text;
	int _position = 0;

	void Next();

	char Current();
};