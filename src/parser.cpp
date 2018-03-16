#include <string>
#include<iostream>
#include "tree.h"
#include "tokenType.h"
#include <stack>
#include <vector>
using namespace std;

class Parser{
	enum TokenType {
		IDENTIFIER_FUNCTION,
		IDENTIFIER_VARIABLE,
		LITERAL_CHAR,
		LITERAL_FLOAT,
		LITERAL_INTEGER,
		LITERAL_STRING,
		KEYWORD_DO,
		KEYWORD_EXECUTE,
		KEYWORD_FORCE,
		KEYWORD_IF,
		KEYWORD_LET,
		KEYWORD_PRETEXT,
		KEYWORD_THEN,
		KEYWORD_TRANSMIT,
		KEYWORD_WHILE,
		KEYWORD_WITH,
		KEYWORD_YOU_MUST,
		KEYWORD_EOF,
		SEPARATOR_LEFT_PAREN,
		SEPARATOR_RIGHT_PAREN,
		SEPARATOR_SEMICOLON,
		OPERATOR_AND,
		OPERATOR_BE,
	  OPERATOR_COMMA,
		OPERATOR_DIVIDE,
		OPERATOR_GT,
		OPERATOR_GTE,
		OPERATOR_IS,
		OPERATOR_LT,
		OPERATOR_LTE,
		OPERATOR_MINUS,
		OPERATOR_MOD,
		OPERATOR_NEQ,
		OPERATOR_OR,
		OPERATOR_PLUS,
		OPERATOR_POW,
		OPERATOR_TIMES,
		WHITESPACE,
		STATEMENT,
		FUNCTION,
		EXPRESSIONNB,
		EXPRESSIONN 
	};
	const vector<vector<Token>> FUNCTIONTOKEN = { {KEYWORD_FORCE, IDENTIFIER_FUNCTION, SEPARATOR_LEFT_PAREN, IDENTIFIER_VARIABLE, SEPARATOR_RIGHT_PAREN, KEYWORD_THEN, STATEMENT ,KEYWORD_YOU_MUST} };
	const vector<vector<Token>> STATEMENTOKEN = {{ KEYWORD_THEN , STATEMENT , KEYWORD_YOU_MUST},
			{KEYWORD_IF , SEPARATOR_LEFT_PAREN , EXPRESSIONNB , SEPARATOR_RIGHT_PAREN , STATEMENT },
			{KEYWORD_DO , STATEMENT , KEYWORD_WHILE , SEPARATOR_LEFT_PAREN , EXPRESSIONNB , SEPARATOR_RIGHT_PAREN },
			{KEYWORD_TRANSMIT , EXPRESSIONNB , SEPARATOR_SEMICOLON },
			{KEYWORD_LET , IDENTIFIER_VARIABLE , OPERATOR_BE , EXPRESSIONNB },
			{EXPRESSIONNB , SEPARATOR_SEMICOLON}}
	Parser(){}
/*
lib -> KEYWORD_PRETEXT . Function* . KEYWORD_EOF
*/
	Tree<Token> lib(stack<Token> &allTokens)   {
		
		Token topItem = allTokens.top();
		if (topItem.type == tokenType.TokenType.KEYWORD_PRETEXT) {
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
Functions -> Keyword_Force . Identifier_Function . Seperator_Left_Paren . Idenifier_Variable* . Seperator_Right_Paren . Keyword_Then . Statement . Keyword_You_must  
*/
	Tree<Token> funcions(stack<Token> &allTokens){
		Tree<Token> funcTree();
		for (int outerLoop = 0; outerLoop < FUNCTIONTOKEN.size(); outerLoop++){
			for (int innerLoop = 0 ; innerLoop < FUNCTIONTOKEN.get(outerLoop).size(); innerLoop++){
				if (FUNCTIONTOKEN.get(outerLoop).get(innerLoop) == Token.IDENTIFIER_VARIABLE){
					while (allTokens.top() == Token.Idenifier_Variable){
						funcTree.AddChild(Tree<Token>(allTokens.top()));
						allTokens.pop();
						if (allTokens.top() == Token.OPERATOR_COMMA){
							allTokens.pop();
						}
					}
				}else if (FUNCTIONTOKEN.get(outerLoop).get(innerLoop) == Token.STATEMENT){
					funcTree.AddChild(Statements(&allTokens));
				}else if (allTokens.top().type == FUNCTIONTOKEN.get(outerLoop).get(innerLoop)){
					//check to see how this works
					funcTree.AddChild(Tree<Token>(allTokens.top()));
				}else{
					break;
				}
				allTokens.pop();
			}
			//Add correction to see if it fails.
		}
		return funcTree;
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
		Tree<Token> statsTree();/*
		for (int outerLoop = 0; outerLoop < FUNCTIONTOKEN.size(); outerLoop++){
			for (int innerLoop = 0 ; innerLoop < FUNCTIONTOKEN.get(outerLoop).size(); innerLoop++){
				if (FUNCTIONTOKEN.get(outerLoop).get(innerLoop) == Token.STATEMENT){
					funcTree.AddChild(Statements(&allTokens));
				}
				if (allTokens.top().type == FUNCTIONTOKEN.get(outerLoop).get(innerLoop)){
					//check to see how this works
					funcTree.AddChild(Tree<Token>(FUNCTIONTOKEN.get(outerLoop).get(innerLoop)));
				}else{
					break;
				}
			}
			//Add correction to see if it fails.
		}
		return funcTree;*/
		return statsTree;
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
