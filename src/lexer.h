#pragma once
#include <iostream>
#include <string>
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