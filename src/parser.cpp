#include <string>
#include<iostream>
#include "tree.h"
#include "tokenType.h"
#include <stack>
#include <vector>
using namespace std;

class Parser{
    /*
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
    */
    
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
void lib(stack<Token> &allTokens, Tree<Token> &parentTree)   {

	queue<Tree<Token> > usedTrees;
	Token top = allTokens.top();
	if (top.type = TokenType.KEYWORD_PRETEXT) {
		Tree<Token> begin(top.value, top.type, top.row, top.column);
		usedTrees.push(begin);
		allTokens.pop();
	} else {
		throw string("Failure on character %s at row %d, column %d.
		Expected the Pretext keyword, I did. This is why you fail. \n",
		 top.value, top.row, top.column);
	}

	while (allTokens.top() != TokenType.KEYWORD_EOF && allTokens.size() > 0) {
		Tree<Token> functionTree("FUNCTION",TokenType.FUNCTION, 0,0);//example of token constructor. row + column doesnt matter in this case because it's just a keyword we're adding
		functions(allTokens, functionTree);
		usedTrees.push(functionTree);
	}

	if (allTokens.top() == TokenType.KEYWORD_EOF) {
		Token top = allTokens.top();
		allTokens.pop();
		Tree<Token> end(top.value, top.type, top.row, top.column);
		usedTrees.push(end);
	} else {
		throw string("Failure: Expected the End of File keyword, I did.
		 			This is why you fail. \n");
	}

	while (usedTree.size() > 0) {
		parentTree.addChild(usedTrees.front());
		usedTrees.pop();
	}
}


/*
Functions -> Keyword_Force . Identifier_Function . Seperator_Left_Paren . Idenifier_Variable* . Seperator_Right_Paren . Keyword_Then . Statement . Keyword_You_must
*/
	void functions(stack<Token> &allTokens, Tree<Token> &parentTree){
		Token top;
		for (int outerLoop = 0; outerLoop < FUNCTIONTOKEN.size(); outerLoop++){
			queue<Tree<Token> > usedTrees ;
			stack<Token> usedTokens;
			bool allPassed = true;
			for (int innerLoop = 0 ; innerLoop < FUNCTIONTOKEN[outerLoop].size(); innerLoop++){
				if (FUNCTIONTOKEN[outerloop][innerLoop] == Token.IDENTIFIER_VARIABLE){
					while (allTokens.size() != 0 && allTokens.top() == Token.IDENTIFIER_VARIABLE){
						//funcTree.AddChild(Tree<Token>(allTokens.top()));
						usedTokens.push(allTokens.top());
						Token temp = allTokens.top();
						Tree<Token> temp2(temp);
						usedTrees.push(temp2) ;
						allTokens.pop();
						if (allTokens.top() == Token.OPERATOR_COMMA){
							usedTokens.push(allTokens.top());
							allTokens.pop();
						}
					}
					continue;
				}else if (allTokens.size() != 0 && FUNCTIONTOKEN[outerloop][innerloop] == Token.STATEMENT){
					//Tree<Token> tempTree = Statements(allTokens);
					//funcTree.AddChild(tempTree);
					//usedTokens.push(allTokens.top());
					//
					//TODO Here is where we have to add the placeholder statement call
					//
					Tree<Token> statementConverter(/* Change this to be placeholder */allTokens.top());
					//allTokens.pop();
					statements(allTokens, statementConverter);
					usedTrees.push(statementConverter) ;
				}else if (allTokens.top().type == FUNCTIONTOKEN[outerloop][innerloop]){
					//check to see how this works
					Token temp = allTokens.top();
					Tree<Token> temp2(temp);
					usedTrees.push(temp2) ;
					//top = allTokens.top();
					//funcTree.AddChild(top);
				}else{
					allPassed = false;
					break;
				}
				if (allTokens.size() != 0){
					allTokens.pop();
				}
			}
			if (allPassed){
				while (usedTrees.size() >0){
					parentTree.addChild(usedTrees.front());
					usedTrees.pop();
					return;
				}
			}
			//Add correction to see if it fails.
		}
		throw string("Failure at character %s, row %d, column %d.
							This is why you fail",
							top.literal, top.row, top.column);

	}
/*
Statements->Keyword_Then . Statement* . Keyword_You_Must |
			Keyword_If . Seperator_Left_Paren . ExpressionNonBinary . Seperator_Right_Paren . Statement |
			Keyword_Do . Statement . Keyword_While . Seperator_Left_Paren . Expression . Separtor_Right_Paren |
			Keyword_Transmit . Expression . Seperator_Semicolon |
			Keyword_Let . Identifier_Variable . Operator_Be . Expression |
			Expression . Seperator_Semicolon
*/
	void statements(stack<Token> &allTokens,Tree<Token> &parentTree){
		Token top;
		for (int outerLoop = 0; outerLoop < STATEMENTOKEN.size(); outerLoop++){
			queue<Tree<Token> > usedTrees ;
			stack<Token> usedTokens;
			bool allPassed = true;
			for (int innerLoop = 0 ; innerLoop < STATEMENTOKEN[outerLoop].size(); innerLoop++){
				if (allTokens.size() != 0 && STATEMENTOKEN[outerloop][innerloop] == Token.STATEMENT){
					//Tree<Token> tempTree = Statements(allTokens);
					//funcTree.AddChild(tempTree);
					if (FUNCTIONTOKEN[outerloop][innerLoop] == Token.STATEMENT){
						while (allTokens.size() != 0 && allTokens.top() == Token.STATEMENT){
							//funcTree.AddChild(Tree<Token>(allTokens.top()));
							//
							//TODO Here is where we have to add the placeholder statement call
							//
							Tree<Token> statementConverter(/* Change this to be placeholder */allTokens.top());
							//allTokens.pop();
							statements(allTokens, statementConverter);
							usedTrees.push(statementConverter) ;

							//usedTokens.push(allTokens.top());
						//	Token temp = allTokens.top();
							//Tree<Token> temp2(temp);
				//			usedTrees.push(temp2) ;
							//allTokens.pop();
							if (allTokens.top() == Token.OPERATOR_COMMA){
								usedTokens.push(allTokens.top());
								allTokens.pop();
							}
						}
					continue;
				}else if (allTokens.size() != 0 && STATEMENTOKEN[outerloop][innerLoop] == Token.EXPRESSIONNB){
					//usedTokens.push(allTokens.top());
					//
					//TODO Here is where we have to add the placeholder expression call
					//
					Tree<string> exprNB(/* Change this to be placeholder */allTokens.top());
					//allTokens.pop();
					EXPRESSIONNB(allTokens, exprNB);
					usedTrees.push(exprNB) ;
				}else if (allTokens.size() != 0 && allTokens.top().type == STATEMENTOKEN[outerloop][innerloop]){
					//check to see how this works
					Token temp = allTokens.top();
					Tree<Token> temp2(temp);
					usedTrees.push(temp2) ;
					//top = allTokens.top();
					//funcTree.AddChild(top);
				}else{
					allPassed = false;
					break;
				}
				if (allTokens.size()  ==0){
					allTokens.pop();
				}
			}
			if (allPassed){
				while (usedTrees.size() >0){
					parentTree.addChild(usedTrees.front());
					usedTrees.pop();
					return;
				}
			}
			//Add correction to see if it fails.
		}
		throw string("Failure at character %s, row %d, column %d.
							This is why you fail",
							top.literal, top.row, top.column);

	}
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
void expressionBinary(stack<Token> &allTokens, Tree<Token> &parentTree){
	queue<Tree<Token> > usedTrees;
	for (int outerLoop = 0; outerLoop < EXPBTOKEN.size(); outerLoop++) {
		for (int innerLoop = 0; innerLoop < EXPBTOKEN.[outerLoop].size(); innerLoop++) {
			if (allTokens.size() > 0 && EXPBTOKEN[outerloop][innerloop] == TokenType.EXPRESSIONNB) {
				Tree<Token> expressionNBTree("EXPNB",TokenType.EXPRESSIONNB,0,0);
				expressionNonBinary(allTokens, expressionNBTree);
				usedTrees.push(expressionNBTree);
				while (usedTree.size() > 0) { //because expB always ends on ExpNB
					parentTree.addChild(usedTrees.front());
					usedTrees.pop();
				}
				return;
			} else {
				top = allTokens.top();
				if (top.type == EXPBTOKEN[outerloop][innerloop]) {
					allTokens.pop();
					Tree<Token> Operator(top.value, top.type, top.row, top.column);
					usedTrees.push(Operator);
				} else {
					break; //no need for used stack here, since expB only 2 tokens long
				}
			}
		} //didnt add a throw message here, because expB can also just be empty, i.e. next character could be a large variety of things.

	}
	while (usedTree.size() > 0) {
		parentTree.addChild(usedTrees.front());
		usedTrees.pop();
	}
	return;
}
/*
ExpNB -> IDENTIFIER_VARIABLE . ExpB |
		 LITERAL_INTEGER . ExpB |
		 LITERAL_STRING . ExpB |
		 LITERAL_CHAR . ExpB |
		 SEPARATOR_LEFT_PAREN . ExpNB . SEPARATOR_RIGHT_PAREN |
		 KEYWORD_EXECUTE . IDENTIFIER_FUNCTION . SEPARATOR_LEFT_PAREN . Params . SEPARATOR_RIGHT_PAREN |
*/
void expressionNonBinary(stack<Token> &allTokens, Tree<Token> &parentTree){
	queue<Tree<Token> > usedTrees;
	stack<Token> usedTokens;

	for (int outerLoop = 0; outerLoop < EXPNBTOKEN.size(); outerloop++) {
		for (int innerLoop = 0; innerLoop < EXPNBTOKEN[outerloop].size();innerloop++) {
			if (allTokens.size() > 0 && EXPBTOKEN[outerloop][innerloop] == TokenType.EXPRESSIONB) {
				Tree<Token> expressionBTree("EXPB",TokenType.EXPRESSIONB,0,0);
				expressionBinary(allTokens,expressionBTree);
				usedTrees.push(expressionBTree);
				while (usedTree.size() > 0) { //because ExpB is always last token in ExpNB
					parentTree.addChild(usedTrees.front());
					usedTrees.pop();
				}
				return;
			} else if (allTokens.size() > 0 && EXPBTOKEN[outerloop][innerloop] == TokenType.EXPRESSIONNB) {
				Tree<Token> expressionNBTree("EXPNB",TokenType.EXPRESSIONNB,0,0);
				expressionNonBinary(allTokens, expressionNBTree);
				usedTrees.push(expressionNBTree);
			} else if (allTokens.size() > 0 && EXPBTOKEN[outerloop][innerloop] == TokenType.PARAMS) { //checking parameters
				if (allTokens.top() != TokenType.SEPERATOR_RIGHT_PAREN) {
					Tree<Token> paramTree("PARAMS",TokenType.PARAMS,0,0);
					expressionNonBinary(allTokens,paramTree);
					usedTrees.push(paramTree);
					while (allTokens.top() != TokenType.SEPERATOR_RIGHT_PAREN) {
						top = allTokens.top();
						if (top.type == TokenType.OPERATOR_COMMA) {
							usedTokens.push(top);
							allTokens.pop();
							Tree<Token> paramTreeMore("PARAMS",TokenType.PARAMS,0,0);
							expressionNonBinary(allTokens,paramTreeMore);
							usedTrees.push(paramTreeMore);
						} else {
							throw string ("Failure at character %d, row %d, column %d.
							Expected a comma or right parenthesis, did I. This is why you fail",
							top.value, top.row, top.column);
						}
					}
					top = allTokens.top();
					usedTokens.push(top);
					allTokens.pop();
					while (usedTree.size() > 0) { //because right paren is always at end of ExpNB
						parentTree.addChild(usedTrees.front());
						usedTrees.pop();
					}
					return;
				} else {
					top = allTokens.top();
					usedTokens.push(top);
					allTokens.pop();
					while (usedTree.size() > 0) { //because right paren is always at end of ExpNB
						parentTree.addChild(usedTrees.front());
						usedTrees.pop();
					}
					return;

				}
			} else {
				top = allTokens.top();
				if (top == EXPNBTOKEN[outerloop][innerloop]) {
					Tree<Token> temp(top.value, top.type,0,0);
					usedTrees.push(temp);
					usedTokens.push(top);
					allTokens.pop();
				} else {
					if (outerloop == EXPNBTOKEN.size()-1) {
						throw string ("Failure at character %s, row %d, column %d.
						Matches a non-binary expression, it does not. This is why you fail",
						top.value, top.row, top.column);
					} else {
						break;
					}
				}
			}
		}
		while (usedTokens.size() != 0) {
			allTokens.push(usedTokens.top());
			usedTokens.pop();
		}
	}
	while (usedTree.size() > 0) {
		parentTree.addChild(usedTrees.front());
		usedTrees.pop();
	}
	return;

}

}
