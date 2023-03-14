#include <memory>
#include "parser.h"

SyntaxToken* Parser::Peek(int offset){
	int index = _position + offset;
	if (index >= _tokens.size()){
		return _tokens[_tokens.size() - 1];
	}

	return _tokens[index];
}

Parser::Parser(std::string text){
	std::vector<SyntaxToken*> tokens;
	Lexer lexer(text);
	SyntaxKind currentTokenKind = EOF_TOKEN;
	SyntaxToken* token = nullptr;
	do{
		token = lexer.Lex();
		currentTokenKind = token->Kind;

		if (token->Kind != WHITESPACE_TOKEN
		&& token->Kind != BAD_TOKEN){
			tokens.push_back(token);
		}
		else{
			delete token;
		}
	} while (currentTokenKind != EOF_TOKEN);

	_tokens = std::move(tokens);

	_diagnostics = std::move(lexer.Diagnostic());
}


SyntaxToken* Parser::NextToken(){
	SyntaxToken* current = Current();
	_position++;
	return current;
}

SyntaxToken* Parser::MatchToken(SyntaxKind kind){
	if (Current()->Kind == kind)
		return NextToken();

	_diagnostics.push_back(std::string("[ERROR]: unexpected token <") + GetSyntaxKindStr(Current()->Kind) + std::string(">, expected <") + GetSyntaxKindStr(kind) + std::string(">"));
	return new SyntaxToken(kind, Current()->Position, "\0");
}

ExpressionSyntax* Parser::ParsePrimaryExpression(){
	if (Current()->Kind == OPAR_TOKEN){
		return new ParenthesizedExpressionSyntax(NextToken(), ParseExpression(), MatchToken(CPAR_TOKEN));
	}
	return new LiteralExpressionSyntax(MatchToken(NUMBER_TOKEN));
}

ExpressionSyntax* Parser::ParseExpression(){
	return ParseTerm();
}

ExpressionSyntax* Parser::ParseTerm(){
	ExpressionSyntax* left = ParseFactor();

	while (Current()->Kind == PLUS_TOKEN
	|| Current()->Kind == MINUS_TOKEN){
		left = new BinaryExpressionSyntax(left, NextToken(), ParseFactor());
	}

	return left;
}

ExpressionSyntax* Parser::ParseFactor(){
	ExpressionSyntax* left = ParsePrimaryExpression();

	while (Current()->Kind == ASTERISK_TOKEN
	|| Current()->Kind == FSLASH_TOKEN){
		left = new BinaryExpressionSyntax(left, NextToken(), ParsePrimaryExpression());
	}

	return left;
}

SyntaxTree* Parser::Parse(){
	return new SyntaxTree(std::move(_diagnostics), ParseExpression(), MatchToken(EOF_TOKEN));
}

int Evaluator::EvaluateExpression(ExpressionSyntax& root){
	return root.GetValue();
}

SyntaxTree* SyntaxTree::Parse(const std::string& text){
	std::unique_ptr<Parser> parser = std::make_unique<Parser>(text);
	SyntaxTree* tree = parser->Parse();
	return tree;
}