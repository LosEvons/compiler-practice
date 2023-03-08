#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <optional>
#include "definitions.h"
#include "exceptions.h"

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
	NUMBER_EXPRESSION_SYNTAX,
	BINARY_EXPRESSION_SYNTAX,
	PARENTHESIZED_EXPRESSION_SYNTAX,
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
	SyntaxKind Kind = BAD_TOKEN;
	int Position = -1;
	std::string Text;
	int Value = -1;
	bool Nullval = true;

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

class ExpressionSyntax : public SyntaxNode{
public:
	virtual int GetValue() { throw tcexc::NotImplementedException(); return 0; };
};

class NumberExpressionSyntax : public ExpressionSyntax{
public:
	SyntaxToken NumberToken;
	SyntaxKind Kind() override { return NUMBER_EXPRESSION_SYNTAX; };
	NumberExpressionSyntax(const SyntaxToken numberToken) : NumberToken(numberToken){};
	std::vector<SyntaxNode*> GetChildren() override { std::vector<SyntaxNode*> childs{&NumberToken}; return childs; };
	int GetValue() override { return NumberToken.Value; };
};

class BinaryExpressionSyntax : public ExpressionSyntax{
public:
	SyntaxKind Kind() override { return BINARY_EXPRESSION_SYNTAX; };
	ExpressionSyntax* Left = nullptr;
	SyntaxToken* OperatorToken = nullptr;
	ExpressionSyntax* Right = nullptr;
	BinaryExpressionSyntax(ExpressionSyntax* left, SyntaxToken* operatorToken, ExpressionSyntax* right);
	std::vector<SyntaxNode*> GetChildren() override { std::vector<SyntaxNode*> childs{Left, OperatorToken, Right}; return childs; };
	int GetValue() override;
};

class ParenthesizedExpressionSyntax : public ExpressionSyntax{
public:
	ParenthesizedExpressionSyntax(SyntaxToken* opToken, ExpressionSyntax* expression, SyntaxToken* cpToken)
		: OPToken(opToken), Expression(expression), CPToken(cpToken){}
	SyntaxKind Kind() override { return PARENTHESIZED_EXPRESSION_SYNTAX; };
	std::vector<SyntaxNode*> GetChildren() override { std::vector<SyntaxNode*> childs{OPToken, Expression, CPToken}; return childs; };
	int GetValue() override { return Expression->GetValue(); };
private:
	SyntaxToken* OPToken = nullptr;
	ExpressionSyntax* Expression = nullptr;
	SyntaxToken* CPToken = nullptr;
};

class SyntaxTree{
public:
	std::vector<std::string>* Diagnostic() { return _diagnostics; };
	ExpressionSyntax* Root = nullptr;
	SyntaxToken* EofToken = nullptr;
	SyntaxTree(std::vector<std::string>* diagnostics, ExpressionSyntax* root, SyntaxToken* eofToken);
	static SyntaxTree* Parse(std::string text);

private:
	std::vector<std::string>* _diagnostics = nullptr;
};

class Lexer{
public:
	Lexer(std::string text);
	SyntaxToken* NextToken();
	std::vector<std::string>* Diagnostic(){ return _diagnostics; };

private:
	std::string _text;
	int _position = 0;
	std::vector<std::string>* _diagnostics = new std::vector<std::string>();

	void Next();
	char Current();
};