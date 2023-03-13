#include <iostream>
#include <string>
#include "lexer.h"
#include <cstring>

std::string GetSyntaxKindStr(SyntaxKind kind){
	switch(kind){
		case BAD_TOKEN:
			return "BAD TOKEN";
			break;
		case EOF_TOKEN:
			return "END OF FILE";
			break;
		case WHITESPACE_TOKEN:
			return "WHITESPACE";
			break;
		case NUMBER_TOKEN:
			return "NUMBER";
			break;
		case PLUS_TOKEN:
			return "PLUS";
			break;
		case MINUS_TOKEN:
			return "MINUS";
			break;
		case ASTERISK_TOKEN:
			return "ASTERISK";
			break;
		case FSLASH_TOKEN:
			return "FORWARD SLASH";
			break;
		case OPAR_TOKEN:
			return "OPEN PARENTHESES";
			break;
		case CPAR_TOKEN:
			return "CLOSE PARENTHESES";
			break;
		case NUMBER_EXPRESSION_SYNTAX:
			return "NUMBER EXPRESSION";
			break;
		case BINARY_EXPRESSION_SYNTAX:
			return "BINARY EXPRESSION";
			break;
		case PARENTHESIZED_EXPRESSION_SYNTAX:
			return "PARENTHESIZED EXPRESSION";
			break;

		default:
			break;
	}
	return "FAILED TO IDENTIFY TOKEN";
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
		int value = 0;
		
		try {
			value = std::stoi(text);
		}
		catch (std::invalid_argument const& ex){
			_diagnostics.push_back(
				std::string("[ERROR]: The number: ") + text + std::string(" isn't a valid Int32.")
			);
		}
		catch (std::out_of_range const& ex){
			_diagnostics.push_back(
				std::string("[ERROR]: The number: ") + text + std::string(" is out of Int32 range.")
			);
		}

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

	_diagnostics.push_back(std::string("[ERROR]: bad character input: ") + Current());
	return new SyntaxToken(BAD_TOKEN, _position++, _text.substr(_position, 1));
}

void Lexer::Next(){
	_position++;
}

char const Lexer::Current() const{
	if (_position >= _text.length())
		return '\0';
	
	return _text[_position];
}

BinaryExpressionSyntax::BinaryExpressionSyntax(ExpressionSyntax* left, SyntaxToken* operatorToken, ExpressionSyntax* right){
	Left = left;
	OperatorToken = operatorToken;
	Right = right;
}

BinaryExpressionSyntax::~BinaryExpressionSyntax(){
	delete Left;
	delete OperatorToken;
	delete Right;
}

ParenthesizedExpressionSyntax::~ParenthesizedExpressionSyntax(){
	delete OPToken;
	delete Expression;
	delete CPToken;
}

SyntaxTree::~SyntaxTree(){
	delete Root;
	delete EofToken;
}

int BinaryExpressionSyntax::GetValue(){
	int left = Left->GetValue();
	int right = Right->GetValue();

	switch(OperatorToken->Kind){
		case PLUS_TOKEN:
			return left + right;
			break;
		case MINUS_TOKEN:
			return left - right;
			break;
		case ASTERISK_TOKEN:
			return left * right;
			break;
		case FSLASH_TOKEN:
			return left / right;
			break;
		
		default:
			std::string errstr = "Unexpected binary operator." + GetSyntaxKindStr(OperatorToken->Kind);
			throw std::invalid_argument(errstr);
			break;
	}
}
