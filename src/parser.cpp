#include <string>
#include<iostream>
#include "tree.h"
#include "tokenType.h"
#include <stack>
#include <vector>
#include <queue>
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

	const vector<vector<TokenType> > FUNCTIONTOKEN = { {TokenType::KEYWORD_FORCE, TokenType::IDENTIFIER_FUNCTION, TokenType::SEPARATOR_LEFT_PAREN, TokenType::IDENTIFIER_VARIABLE, TokenType::SEPARATOR_RIGHT_PAREN, TokenType::KEYWORD_THEN, TokenType::STATEMENT ,TokenType::KEYWORD_YOU_MUST} };
	const vector<vector<TokenType> > STATEMENTOKEN = { { TokenType::KEYWORD_THEN , TokenType::STATEMENT , TokenType::KEYWORD_YOU_MUST},
												  {TokenType::KEYWORD_IF , TokenType::SEPARATOR_LEFT_PAREN , TokenType::EXPRESSIONNB , TokenType::SEPARATOR_RIGHT_PAREN , TokenType::STATEMENT },
												  {TokenType::KEYWORD_DO , TokenType::STATEMENT , TokenType::KEYWORD_WHILE , TokenType::SEPARATOR_LEFT_PAREN , TokenType::EXPRESSIONNB , TokenType::SEPARATOR_RIGHT_PAREN },
												  {KEYWORD_TRANSMIT , EXPRESSIONNB , SEPARATOR_SEMICOLON },
												  {TokenType::KEYWORD_LET , TokenType::IDENTIFIER_VARIABLE , TokenType::OPERATOR_BE , TokenType::EXPRESSIONNB },
												  {TokenType::EXPRESSIONNB , TokenType::SEPARATOR_SEMICOLON}};
	const vector<vector<TokenType> > LIBTOKEN = { {TokenType::FUNCTIONS, TokenType::KEYWORD_THE_END}};
	const vector<vector<TokenType> > EXPBTOKEN = { {TokenType::OPERATOR_PLUS, TokenType::EXPRESSIONNB},
	 										  {TokenType::OPERATOR_TIMES, TokenType::EXPRESSIONNB},
											  {TokenType::OPERATOR_MINUS, TokenType::EXPRESSIONNB},
											  {TokenType::OPERATOR_DIVIDE, TokenType::EXPRESSIONNB},
											  {TokenType::OPERATOR_MOD, TokenType::EXPRESSIONNB},
											  {TokenType::OPERATOR_POW, TokenType::EXPRESSIONNB},
											  {TokenType::OPERATOR_OR, TokenType::EXPRESSIONNB},
											  {TokenType::OPERATOR_AND, TokenType::EXPRESSIONNB},
											  {TokenType::OPERATOR_IS, TokenType::EXPRESSIONNB},
											  {TokenType::EMPTY}};
	const vector<vector<TokenType> > EXPNBTOKEN = { {TokenType::IDENTIFIER_VARIABLE, TokenType::EXPRESSIONB},
											   {TokenType::LITERAL_INTEGER, TokenType::EXPRESSIONB},
											   {TokenType::LITERAL_STRING, TokenType::EXPRESSIONB},
											   {TokenType::LITERAL_CHAR, TokenType::EXPRESSIONB},
											   {TokenType::SEPARATOR_LEFT_PAREN, TokenType::EXPRESSIONNB, TokenType::SEPARATOR_RIGHT_PAREN},
											   {TokenType::KEYWORD_EXECUTE, TokenType::IDENTIFIER_FUNCTION,TokenType::SEPARATOR_LEFT_PAREN, TokenType::PARAMS, TokenType::SEPARATOR_RIGHT_PAREN}};

	Parser(){}
/*
lib -> KEYWORD_PRETEXT . Function* . KEYWORD_EOF
*/
void lib(stack<Token> &allTokens, Tree<Token> &parentTree)   {

	queue<Tree<Token> > usedTrees;
	Token top = allTokens.top();
	if (top.type = TokenType::KEYWORD_PRETEXT) {
		Tree<Token> begin(top);
		usedTrees.push(begin);
		allTokens.pop();
	} else {
		/*throw string("Failure on character %s at row %d, column %d.
		Expected the Pretext keyword, I did. This is why you fail. \n",
		 top.value, top.row, top.column);*/
		throw string("Error in code");
	}

	while (allTokens.size() > 0 && allTokens.top().type != TokenType::KEYWORD_THE_END ) {
		Token FuncTok = {TokenType::FUNCTIONS,"FUNCTION",0,0};
		
		Tree<Token> functionTree(FuncTok);//example of token constructor. row + column doesnt matter in this case because it's just a keyword we're adding
		functions(allTokens, functionTree);
		usedTrees.push(functionTree);
	}

	if (allTokens.top().type == TokenType::KEYWORD_THE_END) {
		Token top = allTokens.top();
		allTokens.pop();
		Tree<Token> end(top);
		usedTrees.push(end);
	} else {
	/*	throw string("Failure: Expected the End of File keyword, I did.
		 			This is why you fail. \n");*/
		throw string("Error in code");
	}

	while (usedTrees.size() > 0) {
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
				if (FUNCTIONTOKEN[outerLoop][innerLoop] == TokenType::IDENTIFIER_VARIABLE){
					while (allTokens.size() != 0 && allTokens.top().type == TokenType::IDENTIFIER_VARIABLE){
						//funcTree.AddChild(Tree<Token>(allTokens.top()));
						usedTokens.push(allTokens.top());
						Token temp = allTokens.top();
						Tree<Token> temp2(temp);
						usedTrees.push(temp2) ;
						allTokens.pop();
						if (allTokens.top().type == TokenType::OPERATOR_COMMA){
							usedTokens.push(allTokens.top());
							allTokens.pop();
						}
					}
					continue;
				}else if (allTokens.size() != 0 && FUNCTIONTOKEN[outerLoop][innerLoop] == TokenType::STATEMENT){
					//Tree<Token> tempTree = Statements(allTokens);
					//funcTree.AddChild(tempTree);
					//usedTokens.push(allTokens.top());
					//
					//TODO Here is where we have to add the placeholder statement call
					//
					Token stateHolder = {TokenType::STATEMENT, "STATEMENT" , 0,0};

					Tree<Token> statementConverter(stateHolder);
					//allTokens.pop();
					statements(allTokens, statementConverter);
					usedTrees.push(statementConverter) ;
				}else if (allTokens.top().type == FUNCTIONTOKEN[outerLoop][innerLoop]){
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
		/*throw string("Failure at character %s, row %d, column %d.
							This is why you fail",
							top.literal, top.row, top.column);*/
		throw string("Error in code");

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
				if (allTokens.size() != 0 && STATEMENTOKEN[outerLoop][innerLoop] == TokenType::STATEMENT){
					//Tree<Token> tempTree = Statements(allTokens);
					//funcTree.AddChild(tempTree);
					if (FUNCTIONTOKEN[outerLoop][innerLoop] == TokenType::STATEMENT){
						while (allTokens.size() != 0 && allTokens.top().type == TokenType::STATEMENT){
							//funcTree.AddChild(Tree<Token>(allTokens.top()));
							//
							//TODO Here is where we have to add the placeholder statement call
							//
							Token stateHolder = {TokenType::STATEMENT, "STATEMENT" , 0,0};

							Tree<Token> statementConverter(stateHolder);
							//allTokens.pop();
							statements(allTokens, statementConverter);
							usedTrees.push(statementConverter) ;

							//usedTokens.push(allTokens.top());
						//	Token temp = allTokens.top();
							//Tree<Token> temp2(temp);
				//			usedTrees.push(temp2) ;
							//allTokens.pop();
							if (allTokens.top().type == TokenType::OPERATOR_COMMA){
								usedTokens.push(allTokens.top());
								allTokens.pop();
							}
						}
					continue;
				}else if (allTokens.size() != 0 && STATEMENTOKEN[outerLoop][innerLoop] == TokenType::EXPRESSIONNB){
					//usedTokens.push(allTokens.top());
					//
					//TODO Here is where we have to add the placeholder expression call
					//
					Token exprTokNB = {TokenType::EXPRESSIONNB, "EXPRESSIONNB", 0,0};

					Tree<Token> exprNB(exprTokNB);
					//allTokens.pop();
					expressionNonBinary(allTokens, exprNB);
					usedTrees.push(exprNB) ;
				}else if (allTokens.size() != 0 && allTokens.top().type == STATEMENTOKEN[outerLoop][innerLoop]){
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
		/*throw string("Failure at character %s, row %d, column %d.
							This is why you fail",
							top.literal, top.row, top.column);*/
		throw string("Error in code");

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
		for (int innerLoop = 0; innerLoop < EXPBTOKEN[outerLoop].size(); innerLoop++) {
			if (allTokens.size() > 0 && EXPBTOKEN[outerLoop][innerLoop] == TokenType::EXPRESSIONNB) {
				Token exprTokNB = {TokenType::EXPRESSIONNB, "EXPRESSIONNB", 0,0};
				Tree<Token> expressionNBTree(exprTokNB);
				expressionNonBinary(allTokens, expressionNBTree);
				usedTrees.push(expressionNBTree);
				// commented out because what if things dont match down stream and then its on the parent
				//while (usedTree.size() > 0) { //because expB always ends on ExpNB
				//	parentTree.addChild(usedTrees.front());
				//	usedTrees.pop();
				//}
				return;
			} else {
				Token top = allTokens.top();
				if (top.type == EXPBTOKEN[outerLoop][innerLoop]) {
					allTokens.pop();
					Tree<Token> Operator(top);
					usedTrees.push(Operator);
				} else {
					break; //no need for used stack here, since expB only 2 tokens long
				}
			}
		} //didnt add a throw message here, because expB can also just be empty, i.e. next character could be a large variety of things.

	}
	while (usedTrees.size() > 0) {
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

	for (int outerLoop = 0; outerLoop < EXPNBTOKEN.size(); outerLoop++) {
		for (int innerLoop = 0; innerLoop < EXPNBTOKEN[outerLoop].size();innerLoop++) {
			if (allTokens.size() > 0 && EXPBTOKEN[outerLoop][innerLoop] == TokenType::EXPRESSIONB) {
				Token exprTokBi = {TokenType::EXPRESSIONB, "EXPRESSIONB", 0,0};
				Tree<Token> expressionBTree(exprTokBi);
				expressionBinary(allTokens,expressionBTree);
				usedTrees.push(expressionBTree);

				while (usedTrees.size() > 0) { //because ExpB is always last token in ExpNB
					parentTree.addChild(usedTrees.front());
					usedTrees.pop();
				}
				return;
			} else if (allTokens.size() > 0 && EXPBTOKEN[outerLoop][innerLoop] == TokenType::EXPRESSIONNB) {
				Token exprTokNB = {TokenType::EXPRESSIONNB, "EXPRESSIONNB", 0,0};
				Tree<Token> expressionNBTree(exprTokNB);
				usedTrees.push(expressionNBTree);
			} else if (allTokens.size() > 0 && EXPBTOKEN[outerLoop][innerLoop] == TokenType::PARAMS) { //checking parameters
				if (allTokens.top().type != TokenType::SEPARATOR_RIGHT_PAREN) {
					Token paramTok = {TokenType::PARAMS,"PARAMS",0,0};
					Tree<Token> paramTree(paramTok);
					expressionNonBinary(allTokens,paramTree);
					usedTrees.push(paramTree);
					while (allTokens.top().type != TokenType::SEPARATOR_RIGHT_PAREN) {
						Token top = allTokens.top();
						if (top.type == TokenType::OPERATOR_COMMA) {
							usedTokens.push(top);
							allTokens.pop();
							Token paramTok = {TokenType::PARAMS,0,0};
							Tree<Token> paramTreeMore(paramTok);
							expressionNonBinary(allTokens,paramTreeMore);
							usedTrees.push(paramTreeMore);
						} else {
						/*	throw string ("Failure at character %d, row %d, column %d.
							Expected a comma or right parenthesis, did I. This is why you fail",
							top.value, top.row, top.column);*/
							throw string("Error in code");
						}
					}
					Token top = allTokens.top();
					usedTokens.push(top);
					allTokens.pop();
					while (usedTrees.size() > 0) { //because right paren is always at end of ExpNB
						parentTree.addChild(usedTrees.front());
						usedTrees.pop();
					}
					return;
				} else {
					Token top = allTokens.top();
					usedTokens.push(top);
					allTokens.pop();
					while (usedTrees.size() > 0) { //because right paren is always at end of ExpNB
						parentTree.addChild(usedTrees.front());
						usedTrees.pop();
					}
					return;

				}
			} else {
				Token top = allTokens.top();
				if (top.type == EXPNBTOKEN[outerLoop][innerLoop]) {
					Tree<Token> temp(top);
					usedTrees.push(temp);
					usedTokens.push(top);
					allTokens.pop();
				} else {
					if (outerLoop == EXPNBTOKEN.size()-1) {
						/*throw string ("Failure at character %s, row %d, column %d.
						Matches a non-binary expression, it does not. This is why you fail",
						top.value, top.row, top.column);*/
						throw string("Error in code");
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
	while (usedTrees.size() > 0) {
		parentTree.addChild(usedTrees.front());
		usedTrees.pop();
	}
	return;

    }


};