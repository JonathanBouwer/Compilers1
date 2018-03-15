#include <string>
#include<iostream>
#include "tree.h"
#include "tokenType.h"
#include <stack>
#include <vector>
using namespace std;

class Parser{
	Parser(){}
/*
lib -> KEYWORD_PRETEXT . Function* . KEYWORD_EOF
*/
	Tree<Token> lib(stack<Token> &allTokens)   {
		
		Token topItem = allTokens.top();
		if (topItem.type == tokenType.KEYWORD_PRETEXT) {
			Tree<Token> parseTree(topItem);
			allTokens.pop();
			parseTree.addChild(functions(allTokens));
		} else {
			/*To do throw just throw it and use the keyword string - like 
			creating a string variable as a class. The catch will be in the main
			and all code will be ditched from there.
			*/
			throw string("Failure on character %s at row %d, column %d. Expected the Pretext keyword, I did. This is why you fail. \n", topItem.literal, topItem.row, topItem.column)
		}
		
		

	}

/*
Functions -> Keyword_Execute . Identifier_Function . Seperator_Left_Paren . Idenifier_Variable* . Seperator_Right_Paren . Keyword_Then . Statement . Keyword_You_must  
*/
	Tree<Token> funcions(stack<Token> &allTokens){
		
	}
/*
Statements->Keyword_Then . Statement* . Keyword_You_Must |
			Keyword_If . Seperator_Left_Paren . ExpressionNonBinary . Seperator_Right_Paren . Statement |
			Keyword_Do . Statement . Keyword_While . Seperator_Left_Paren . Expression . Separtor_Right_Paren |
			Keyword_Transmit . Expression . Seperator_Semicolon |
			Keyword_Let . Identifier_Variable . Operator_Be . Expression |
			Expression . Seperator_Semicolon
*/
	Tree<Token> statements(stack<Token> &allTokens){

	}
/*
ExpB -> OPERATOR_PLUS . ExpNB |
		OPERATOR_TIMES . ExpNB |
		OPERATOR_MINUS . ExpNB |
		OPERATOR_DIVIDE . ExpNB |
		OPERATOR_MOD . ExpNB |
		OPERATOR_POW . ExpNB |
		OPERATOR_OR . ExpNB |
		OPERATOR_AND . ExpNB |
		OPERATOR_IS . ExpNB |
		e
*/
	Tree<Token> expressionBinary(stack<Token> &allTokens){

	}
/*
ExpNB -> IDENTIFIER_VARIABLE . ExpB |
		 LITERAL_INTEGER . ExpB |
		 LITERAL_STRING . ExpB |
		 LITERAL_CHAR . ExpB |
		 SEPARATOR_LEFT_PAREN . ExpNB . SEPARATOR_RIGHT_PAREN |
		 KEYWORD_EXECUTE . IDENTIFIER_FUNCTION . SEPARATOR_LEFT_PAREN . ExpNB . SEPARATOR_RIGHT_PAREN |
		 SEPARATOR_LEFT_PAREN . ExpNB . SEPARATOR_RIGHT_PAREN
*/
	Tree<Token> expressionNonBinary(stack<Token> &allTokens){

	}
}
