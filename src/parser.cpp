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
	Lexer* lexer = new Lexer(text);
	SyntaxToken* token;
	do{
		token = lexer->NextToken();

		if (token->Kind != WHITESPACE_TOKEN
		&& token->Kind != BAD_TOKEN){
			tokens.push_back(token);
		}

	} while (token->Kind != EOF_TOKEN);

	_tokens = tokens;

	std::vector<std::string>* srcVec = lexer->Diagnostic();
	_diagnostics->insert(_diagnostics->end(), srcVec->begin(), srcVec->end());
	delete srcVec;
}

SyntaxToken* Parser::NextToken(){
	SyntaxToken* current = Current();
	_position++;
	return current;
}

SyntaxToken* Parser::Match(SyntaxKind kind){
	if (Current()->Kind == kind)
		return NextToken();

	_diagnostics->push_back(std::string("[ERROR]: unexpected token <") + GetSyntaxKindStr(Current()->Kind) + std::string(">, expected <") + GetSyntaxKindStr(kind) + std::string(">"));
	return new SyntaxToken(kind, Current()->Position, "\0");
}

ExpressionSyntax* Parser::ParsePrimaryExpression(){
	if (Current()->Kind == OPAR_TOKEN){
		SyntaxToken* left = NextToken();
		ExpressionSyntax* expression = ParseExpression();
		SyntaxToken* right = Match(CPAR_TOKEN);

		return new ParenthesizedExpressionSyntax(left, expression, right);
	}

	NumberExpressionSyntax numberToken = *Match(NUMBER_TOKEN);
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
	return new SyntaxTree(_diagnostics, expression, eofToken);
}

int Evaluator::EvaluateExpression(ExpressionSyntax* root){
	return root->GetValue();
}

SyntaxTree* SyntaxTree::Parse(std::string text){
	Parser* parser = new Parser(text);
	SyntaxTree* tree = parser->Parse();
	delete parser;
	return tree;
}