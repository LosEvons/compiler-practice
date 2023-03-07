#include <iostream>
#include <string>
#include "lexer.h"

Lexer::Lexer(std::string text){
	_text = text;
}

SyntaxToken::SyntaxToken(SyntaxKind kind, int position, std::string text, bool nullval, int value){
	Kind = kind;
	Position = position;
	Text = text;
	Value = value;
	Nullval = nullval;
}

SyntaxToken* Lexer::NextToken(){
	if (_position >= _text.length()){
		return new SyntaxToken(EOF_TOKEN, _position, "\0");
	}

	else if (isdigit(Current())){
		int start = _position;

		while (isdigit(Current())){
			Next();
		}

		int length = _position - start;
		std::string text = _text.substr(start, length);
		int value = std::stoi(text);
		return new SyntaxToken(NUMBER_TOKEN, start, text, TOKEN_HAS_VALUE, value);
	}

	else if (isspace(Current())){
		int start = _position;

		while (isspace(Current())){
			Next();
		}

		int length = _position - start;
		std::string text = _text.substr(start, length);
		return new SyntaxToken(WHITESPACE_TOKEN, start, text);
	}

	else if (Current() == '+'){
		return new SyntaxToken(PLUS_TOKEN, _position++, "+");
	}
	else if (Current() == '-'){
		return new SyntaxToken(MINUS_TOKEN, _position++, "-");
	}
	else if (Current() == '*'){
		return new SyntaxToken(ASTERISK_TOKEN, _position++, "*");
	}
	else if (Current() == '/'){
		return new SyntaxToken(FSLASH_TOKEN, _position++, "/");
	}
	else if (Current() == '('){
		return new SyntaxToken(OPAR_TOKEN, _position++, "(");
	}
	else if (Current() == ')'){
		return new SyntaxToken(CPAR_TOKEN, _position++, ")");
	}

	return new SyntaxToken(BAD_TOKEN, _position++, _text.substr(_position, 1));
}

void Lexer::Next(){
	_position++;
}

char Lexer::Current(){
	if (_position >= _text.length())
		return '\0';
	
	return _text[_position];
}

BinaryExpressionSyntax::BinaryExpressionSyntax(ExpressionSyntax* left, SyntaxToken* operatorToken, ExpressionSyntax* right){
		Left = left;
		OperatorToken = operatorToken;
		Right = right;
	}