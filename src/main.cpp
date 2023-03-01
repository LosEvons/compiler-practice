#include <iostream>
#include <string>
#include "lexer.h"

int main(){
	while(true)
	{
		std::cout << ">";
		std::string input;
		std::cin >> input;

		Lexer* lexer = new Lexer(input);
		while(true){
			SyntaxToken* token = lexer->NextToken();
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

			delete token;
		}

		delete lexer;
	}
}