#include <iostream>
#include <string>
#include <algorithm>
#include "lexer.h"
#include "parser.h"
#include "definitions.h"
#include "utils.h"

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
	bool showTree = false;

	while(true)
	{
		std::cout << ">";
		std::string input;
		std::getline(std::cin, input);
		if (input.empty()){
			return 0;
		}
		if (input == "#showtree"){
			showTree = !showTree;
			std::string msg = showTree ? "Parse tree visible" : "Parse tree hidden";
			std::cout << msg << std::endl;
			continue;
		}
		else if (input == "#cls"){
			ClearConsole();
			continue;
		}


		SyntaxTree* syntaxTree = SyntaxTree::Parse(input);

		if (showTree){
			PrettyPrint(syntaxTree->Root);
		}
		
		if (syntaxTree->Diagnostic()->size() == 0){
			Evaluator* e = new Evaluator(syntaxTree->Root);
			int results = e->Evaluate();
			delete e;
			std::cout << results << std::endl;
		}
		else{
			auto print = [](const std::string &str) { std::cout << str << std::endl; };
			std::for_each(
				syntaxTree->Diagnostic()->cbegin(), 
				syntaxTree->Diagnostic()->cend(), 
				print);
		}
	}
}

