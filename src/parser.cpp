#include <string>
#include<iostream>
#include "tree.h"
#include "tokenType.h"
#include <stack>
#include <vector>
using namespace std;

class Parser{
	Parser(){}

	Tree<Token> lib(stack<Token> &allTokens){
		Token topItem = allTokens.top();
		if (topItem.type == tokenType.KEYWORD_PRETEXT) {
			Tree<Token> parseTree(topItem);
			allTokens.pop();
			parseTree.addChild(functions(allTokens));
		} else {
			printf("Failure on character %s at row %d, column %d. Expected the Pretext keyword, I did. This is why you fail. \n", topItem.literal, topItem.row, topItem.column);
			exit(0);
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

	Tree<Token> expressionBinary(stack<Token> &allTokens){

	}

	Tree<Token> expressionNonBinary(stack<Token> &allTokens){

	}
}
