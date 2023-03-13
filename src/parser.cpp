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
	do{
		SyntaxToken* token = lexer.NextToken();
		currentTokenKind = token->Kind;

		if (token->Kind != WHITESPACE_TOKEN
		&& token->Kind != BAD_TOKEN){
			tokens.push_back(token);
		}
	} while (currentTokenKind != EOF_TOKEN);

	_tokens = tokens;

	_diagnostics = lexer.Diagnostic();
}


SyntaxToken* Parser::NextToken(){
	SyntaxToken* current = Current();
	_position++;
	return current;
}

SyntaxToken* Parser::Match(SyntaxKind kind){
	if (Current()->Kind == kind)
		return NextToken();

	_diagnostics.push_back(std::string("[ERROR]: unexpected token <") + GetSyntaxKindStr(Current()->Kind) + std::string(">, expected <") + GetSyntaxKindStr(kind) + std::string(">"));
	return new SyntaxToken(kind, Current()->Position, "\0");
}

ExpressionSyntax* Parser::ParsePrimaryExpression(){
	if (Current()->Kind == OPAR_TOKEN){
		SyntaxToken* left = NextToken();
		ExpressionSyntax* expression = ParseExpression();
		SyntaxToken* right = Match(CPAR_TOKEN);

		ExpressionSyntax* returnValue = new ParenthesizedExpressionSyntax(left, expression, right);
		return returnValue;
	}

	SyntaxToken* numberToken = Match(NUMBER_TOKEN);
	return new NumberExpressionSyntax(numberToken);
}

ExpressionSyntax* Parser::ParseExpression(){
	return ParseTerm();
}

ExpressionSyntax* Parser::ParseTerm(){
	ExpressionSyntax* left = ParseFactor();

	while (Current()->Kind == PLUS_TOKEN
	|| Current()->Kind == MINUS_TOKEN){
		SyntaxToken* operatorToken = NextToken();
		ExpressionSyntax* right = ParseFactor();
		left = new BinaryExpressionSyntax(left, operatorToken, right);
	}

	return left;
}

ExpressionSyntax* Parser::ParseFactor(){
	ExpressionSyntax* left = ParsePrimaryExpression();

	while (Current()->Kind == ASTERISK_TOKEN
	|| Current()->Kind == FSLASH_TOKEN){
		SyntaxToken* operatorToken = NextToken();
		ExpressionSyntax* right = ParsePrimaryExpression();
		left = new BinaryExpressionSyntax(left, operatorToken, right);
	}

	return left;
}

SyntaxTree* Parser::Parse(){
	ExpressionSyntax* expression = ParseTerm();
	SyntaxToken* eofToken = Match(EOF_TOKEN);
	SyntaxTree* returnValue = new SyntaxTree(std::move(_diagnostics), expression, eofToken);
	return returnValue;
}

int Evaluator::EvaluateExpression(ExpressionSyntax* root){
	return root->GetValue();
}

SyntaxTree* SyntaxTree::Parse(std::string text){
	std::unique_ptr<Parser> parser = std::make_unique<Parser>(text);
	SyntaxTree* tree = parser->Parse();
	return tree;
}