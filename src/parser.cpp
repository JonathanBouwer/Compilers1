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
		EXPRESSIONB,
		PARAMS,
	};

	/*
	ExpNB -> IDENTIFIER_VARIABLE . ExpB |
			 LITERAL_INTEGER . ExpB |
			 LITERAL_STRING . ExpB |
			 LITERAL_CHAR . ExpB |
			 SEPARATOR_LEFT_PAREN . ExpNB . SEPARATOR_RIGHT_PAREN |
			 KEYWORD_EXECUTE . IDENTIFIER_FUNCTION . SEPARATOR_LEFT_PAREN . ExpNB . SEPARATOR_RIGHT_PAREN |
			 SEPARATOR_LEFT_PAREN . ExpNB . SEPARATOR_RIGHT_PAREN
	*/

	const vector<vector<Token>> FUNCTIONTOKEN = { {KEYWORD_FORCE, IDENTIFIER_FUNCTION, SEPARATOR_LEFT_PAREN, IDENTIFIER_VARIABLE, SEPARATOR_RIGHT_PAREN, KEYWORD_THEN, STATEMENT ,KEYWORD_YOU_MUST} };
	const vector<vector<Token>> STATEMENTOKEN = { { KEYWORD_THEN , STATEMENT , KEYWORD_YOU_MUST},
												  {KEYWORD_IF , SEPARATOR_LEFT_PAREN , EXPRESSIONNB , SEPARATOR_RIGHT_PAREN , STATEMENT },
												  {KEYWORD_DO , STATEMENT , KEYWORD_WHILE , SEPARATOR_LEFT_PAREN , EXPRESSIONNB , SEPARATOR_RIGHT_PAREN },
												  {KEYWORD_TRANSMIT , EXPRESSIONNB , SEPARATOR_SEMICOLON },
												  {KEYWORD_LET , IDENTIFIER_VARIABLE , OPERATOR_BE , EXPRESSIONNB },
												  {EXPRESSIONNB , SEPARATOR_SEMICOLON}};
	const vector<vector<Token>> LIBTOKEN = { {FUNCTIONS, KEYWORD_EOF}};
	const vector<vector<Token>> EXPBTOKEN = { {OPERATOR_PLUS, EXPRESSIONNB},
	 										  {OPERATOR_TIMES, EXPRESSIONNB},
											  {OPERATOR_MINUS, EXPRESSIONNB},
											  {OPERATOR_DIVIDE, EXPRESSIONNB},
											  {OPERATOR_MOD, EXPRESSIONNB},
											  {OPERATOR_POW, EXPRESSIONNB},
											  {OPERATOR_OR, EXPRESSIONNB},
											  {OPERATOR_AND, EXPRESSIONNB},
											  {OPERATOR_IS, EXPRESSIONNB},
											  {EMPTY}};
	const vector<vector<Token>> EXPNBTOKEN = { {IDENTIFIER_VARIABLE, EXPRESSIONB},
											   {LITERAL_INTEGER, EXPRESSIONB},
											   {LITERAL_STRING, EXPRESSIONB},
											   {LITERAL_CHAR, EXPRESSIONB},
											   {SEPERATOR_LEFT_PAREN, EXPRESSIONNB, SEPERATOR_RIGHT_PAREN},
											   {KEYWORD_EXECUTE, IDENTIFIER_FUNCTION,SEPERATOR_LEFT_PAREN, PARAMS, SEPERATOR_RIGHT_PAREN}};

	Parser(){}
/*
lib -> KEYWORD_PRETEXT . Function* . KEYWORD_EOF
*/
Tree<Token> lib(stack<Token> &allTokens)   {

	Tree<Token> retTree();
	Token top = allTokens.top();
	if (top.type = TokenType.KEYWORD_PRETEXT) {
		retTree.addChild(Tree<Token>(top));
		allTokens.pop();
	} else {
		throw string("Failure on character %s at row %d, column %d.
		Expected the Pretext keyword, I did. This is why you fail. \n",
		 top.literal, top.row, top.column);
	}

	while (allTokens.top() != TokenType.KEYWORD_EOF && allTokens.size() > 0) {
		retTree.addChild(functions(&allTokens));
	}

	if (allTokens.top() == TokenType.KEYWORD_EOF) {
		allTokens.pop();
		return retTree;
	} else {
		throw string("Failure: Expected the End of File keyword, I did.
		 			This is why you fail. \n");
	}
}


/*
Functions -> Keyword_Force . Identifier_Function . Seperator_Left_Paren . Idenifier_Variable* . Seperator_Right_Paren . Keyword_Then . Statement . Keyword_You_must
*/
	Tree<Token> functions(stack<Token> &allTokens){
		Tree<Token> funcTree();
		stack<Token> usedTokens;
		Token top;
		for (int outerLoop = 0; outerLoop < FUNCTIONTOKEN.size(); outerLoop++){
			for (int innerLoop = 0 ; innerLoop < FUNCTIONTOKEN[outerLoop].size(); innerLoop++){
				if (FUNCTIONTOKEN[outerloop][innerLoop] == Token.IDENTIFIER_VARIABLE){
					while (allTokens.top() == Token.IDENTIFIER_VARIABLE){
						funcTree.AddChild(Tree<Token>(allTokens.top()));
						usedTokens.push(allTokens.top());
						allTokens.pop();
						if (allTokens.top() == Token.OPERATOR_COMMA){
							usedTokens.push(allTokens.top());
							allTokens.pop();
						}
					}
					continue;
				}else if (FUNCTIONTOKEN[outerloop][innerloop] == Token.STATEMENT){
					Tree<Token> tempTree = Statements(allTokens);
					funcTree.AddChild(tempTree);
				}else if (allTokens.top().type == FUNCTIONTOKEN[outerloop][innerloop]){
					//check to see how this works
					top = allTokens.top();
					funcTree.AddChild(top);
				}else{
					break;
				}
				allTokens.pop();
			}
			if (allTokens.size() != 0){
				
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
		Tree<Token> expB();
		Token top; //not sure how token will be initialized
		for (int outerLoop = 0; outerLoop < EXPBTOKEN.size(); outerLoop++) {
			for (int innerLoop = 0; innerLoop < EXPBTOKEN.[outerLoop].size(); innerLoop++) {
				if (allTokens.size() > 0 && EXPBTOKEN[outerloop][innerloop] == TokenType.EXPRESSIONNB) {
					expB.addChild(expressionNonBinary(&allTokens));
					return expB;
				} else {
					top = allTokens.top();
					if (top.type == EXPBTOKEN[outerloop][innerloop]) {
						allTokens.pop();
						expB.addChild(Tree<Token>(top));
					} else {
						break; //no need for used stack here, since expB only 2 tokens long
					}
				}
			} //didnt add a throw message here, because expB can also just be empty, i.e. next character could be a large variety of things.

		}
		return expB;
	}
/*
ExpNB -> IDENTIFIER_VARIABLE . ExpB |
		 LITERAL_INTEGER . ExpB |
		 LITERAL_STRING . ExpB |
		 LITERAL_CHAR . ExpB |
		 SEPARATOR_LEFT_PAREN . ExpNB . SEPARATOR_RIGHT_PAREN |
		 KEYWORD_EXECUTE . IDENTIFIER_FUNCTION . SEPARATOR_LEFT_PAREN . Params . SEPARATOR_RIGHT_PAREN |
*/
	Tree<Token> expressionNonBinary(stack<Token> &allTokens){
		Tree<Token> expNB();
		stack<Token> usedTokens;
		Token top; //not sure how token will be initialized

		for (int outerLoop = 0; outerLoop < EXPNBTOKEN.size(); outerloop++) {
			for (int innerLoop = 0; innerLoop < EXPNBTOKEN[outerloop].size();innerloop++) {
				if (allTokens.size() > 0 && EXPBTOKEN[outerloop][innerloop] == TokenType.EXPRESSIONB) {
					expNB.addChild(expressionBinary(&allTokens));
					return expNB; //because ExpB is always at the end of ExpNB
				} else if (allTokens.size() > 0 && EXPBTOKEN[outerloop][innerloop] == TokenType.EXPRESSIONNB) {
					expNB.addChild(expressionNonBinary(&allTokens));
				} else if (allTokens.size() > 0 && EXPBTOKEN[outerloop][innerloop] == TokenType.PARAMS) { //checking parameters
					if (allTokens.top() != TokenType.SEPERATOR_RIGHT_PAREN) {
						expNB.addChild(expressionNonBinary(&allTokens));
						while (allTokens.top() != TokenType.SEPERATOR_RIGHT_PAREN) {
							top = allTokens.top();
							if (top.type == TokenType.OPERATOR_COMMA) {
								allTokens.pop();
								expNB.addChild(expressionNonBinary(&allTokens));
							} else {
								throw string ("Failure at character %d, row %d, column %d.
								Expected a comma or right parenthesis, did I. This is why you fail",
								top.literal, top.row, top.column);
							}
						}
						top = allTokens.top();
						allTokens.pop();
						expNB.addChild(Tree<Token>(top));
						return expNB; //because in expNB, after params it's only right paren and then end
					} else {
						top = allTokens.top();
						allTokens.pop();
						expNB.addChild(Tree<Token>(top));
						return expNB; //because in expNB, after params it's only right paren and then end
					}
				} else {
					top = allTokens.top();
					if (top == EXPNBTOKEN[outerloop][innerloop]) {
						expNB.addChild(Tree<Token>(top));
						usedTokens.push(top);
						allTokens.pop();
					} else {
						if (outerloop == EXPNBTOKEN.size()-1) {
							throw string ("Failure at character %s, row %d, column %d.
							Matches a non-binary expression, it does not. This is why you fail",
							top.literal, top.row, top.column);
						} else {
							while (usedTokens.size() != 0) {
								allTokens.push(usedTokens.top());
								usedTokens.pop();
							}
						}
					}
				}
			}
		}
		return expNB;

	}

}
