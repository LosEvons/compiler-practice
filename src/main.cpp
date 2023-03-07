#include <iostream>
#include <string>
#include <algorithm>
#include "lexer.h"
#include "parser.h"
#include "definitions.h"

void PrettyPrint(SyntaxNode* node, std::string indent = "", bool isLast = true){
	std::string marker = isLast ? "+---" : "|---";
	std::cout << indent;
	std::cout << marker;
	node->PrintClassContents();
	SyntaxNode* lastChild;
	if (node->GetChildren().size() > 0)
		lastChild = node->GetChildren().back();
	indent += isLast ? "    " : "|   ";

	for (int i = 0; i < node->GetChildren().size(); i++){
		PrettyPrint(node->GetChildren()[i], indent, node->GetChildren()[i] == lastChild);
	}
}

int main(){
	std::cout << "STANDARD COLOR SCHEME: " 
	<< ANSI_COLOR_BLUE << "EXPRESSION " 
	<< ANSI_COLOR_RED << "TOKEN " 
	<< ANSI_COLOR_YELLOW << "INFO " 
	<< ANSI_COLOR_RESET << std::endl;

	std::cout << "STANDARD TREE FORMAT: \n" << "TYPE : ?STRING : ?VALUE" << std::endl;
	while(true)
	{
		std::cout << ">";
		std::string input;
		std::getline(std::cin, input);
		if (input.empty()){
			return 0;
		}
		Parser* parser = new Parser(input);
		ExpressionSyntax* expression = parser->Parse();

		PrettyPrint(expression);

		Lexer* lexer = new Lexer(input);
		SyntaxToken* token;
		
		if (parser->Diagnostic().size() > 0){
			auto print = [](const std::string &str) { std::cout << str << std::endl; };
			std::for_each(
				parser->Diagnostic().cbegin(), 
				parser->Diagnostic().cend(), 
				print);
		}	
		
		/*
		while(true){
			token = lexer->NextToken();
			if (token->Kind == EOF_TOKEN){
				break;
			}
			std::cout 
			<< "Token kind: " << token->Kind
			<< " Token text: " << token->Text;
			if (!token->Nullval){
				std::cout
				<< " Token value: " << token->Value;
			}
			std::cout << std::endl;
		}
		*/

		delete parser;
		delete token;
		delete lexer;
	}
}

