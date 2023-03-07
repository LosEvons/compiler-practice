#include "parser.h"

SyntaxToken* Parser::Peek(int offset){
	int index = _position + offset;
	if (index >= _tokens.size()){
		return &_tokens[_tokens.size() - 1];
	}

	return &_tokens[index];
}

Parser::Parser(std::string text){
	std::vector<SyntaxToken> tokens;
	Lexer* lexer = new Lexer(text);
	SyntaxToken* token;
	do{
		token = lexer->NextToken();

		if (token->Kind != WHITESPACE_TOKEN
		&& token->Kind != BAD_TOKEN){
			tokens.push_back(*token);
		}

	} while (token->Kind != EOF_TOKEN);

	_tokens = tokens; //| std::ranges::to<std::vector>();

}

SyntaxToken* Parser::NextToken(){
	SyntaxToken* current = Current();
	_position++;
	return current;
}

SyntaxToken* Parser::Match(SyntaxKind kind){
	if (Current()->Kind == kind)
		return NextToken();

	return new SyntaxToken(kind, Current()->Position, "\0");
}

ExpressionSyntax* Parser::Parse(){
	ExpressionSyntax* left = ParsePrimaryExpression();

	while (Current()->Kind == PLUS_TOKEN
	|| Current()->Kind == MINUS_TOKEN){
		SyntaxToken* operatorToken = NextToken();
		ExpressionSyntax* right = ParsePrimaryExpression();
		left = new BinaryExpressionSyntax(left, operatorToken, right);
	}

	return left;
}

ExpressionSyntax* Parser::ParsePrimaryExpression(){
	NumberExpressionSyntax numberToken = *Match(NUMBER_TOKEN);
	return new NumberExpressionSyntax(numberToken);
}