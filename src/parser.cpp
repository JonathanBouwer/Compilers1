#include <string>
#include<iostream>
#include <stack>
#include <vector>
#include <queue>

#include "tree.h"
#include "tokenType.h"

using namespace std;


const vector<vector<TokenType> > FUNCTION_TOKEN = { {KEYWORD_FORCE, IDENTIFIER_FUNCTION, SEPARATOR_LEFT_PAREN, IDENTIFIER_VARIABLE, SEPARATOR_RIGHT_PAREN, KEYWORD_THEN, STATEMENT ,KEYWORD_YOU_MUST} };
const vector<vector<TokenType> > STATEMENT_TOKEN = { {KEYWORD_THEN , STATEMENT , KEYWORD_YOU_MUST},
											       {KEYWORD_IF , SEPARATOR_LEFT_PAREN , EXPRESSIONNB , SEPARATOR_RIGHT_PAREN , STATEMENT },
											       {KEYWORD_DO , STATEMENT , KEYWORD_WHILE , SEPARATOR_LEFT_PAREN , EXPRESSIONNB , SEPARATOR_RIGHT_PAREN },
											       {KEYWORD_TRANSMIT , EXPRESSIONNB , SEPARATOR_SEMICOLON },
											       {KEYWORD_LET , IDENTIFIER_VARIABLE , OPERATOR_BE , EXPRESSIONNB , SEPARATOR_SEMICOLON },
											       {EXPRESSIONNB , SEPARATOR_SEMICOLON}};
const vector<vector<TokenType> > LIB_TOKEN = {{KEYWORD_PRETEXT, FUNCTIONS, KEYWORD_THE_END}};
const vector<vector<TokenType> > EXPB_TOKEN = {{OPERATOR_PLUS, EXPRESSIONNB},
 										     {OPERATOR_TIMES, EXPRESSIONNB},
										     {OPERATOR_MINUS, EXPRESSIONNB},
										     {OPERATOR_DIVIDE, EXPRESSIONNB},
										     {OPERATOR_MOD, EXPRESSIONNB},
										     {OPERATOR_POW, EXPRESSIONNB},
										     {OPERATOR_OR, EXPRESSIONNB},
										     {OPERATOR_AND, EXPRESSIONNB},
										     {OPERATOR_IS, EXPRESSIONNB},
										     {OPERATOR_GT, EXPRESSIONNB},
										     {OPERATOR_GTE, EXPRESSIONNB},
										     {OPERATOR_LT, EXPRESSIONNB},
										     {OPERATOR_LTE, EXPRESSIONNB},
										     {OPERATOR_NEQ, EXPRESSIONB},
										     {EMPTY}};
const vector<vector<TokenType> > EXPNB_TOKEN = { {IDENTIFIER_VARIABLE, EXPRESSIONB},
										      {LITERAL_INTEGER, EXPRESSIONB},
										      {LITERAL_STRING, EXPRESSIONB},
										      {LITERAL_CHAR, EXPRESSIONB},
										      {SEPARATOR_LEFT_PAREN, EXPRESSIONNB, SEPARATOR_RIGHT_PAREN, EXPRESSIONB},
										      {KEYWORD_EXECUTE, IDENTIFIER_FUNCTION,SEPARATOR_LEFT_PAREN, PARAMS, SEPARATOR_RIGHT_PAREN}};

void expressionNonBinary(stack<Token> &allTokens, Tree<string> &parentTree);

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
void expressionBinary(stack<Token> &allTokens, Tree<string> &parentTree){ //logic for generation of an expressionBinary non-terminal

	queue<Tree<string> > usedTrees;

	for (int outerLoop = 0; outerLoop < EXPB_TOKEN.size(); outerLoop++) {

		for (int innerLoop = 0; innerLoop < EXPB_TOKEN[outerLoop].size(); innerLoop++) {

			if (allTokens.size() > 0 && EXPB_TOKEN[outerLoop][innerLoop] == EXPRESSIONNB) {

				Token exprTokNB = {EXPRESSIONNB, "EXPRESSIONNB", 0,0};
				Tree<string> expressionNBTree(exprTokNB.literal);
				expressionNonBinary(allTokens, expressionNBTree);
				usedTrees.push(expressionNBTree);

				while (usedTrees.size() > 0) {
					parentTree.addChild(usedTrees.front());
					usedTrees.pop();
				}

				return;  //the while loop above + the return statement was added here because ExpressionNonBinary is always at the end of the ExpressionBinary grammar

			} else {

				Token top = allTokens.top();

				if (top.type == EXPB_TOKEN[outerLoop][innerLoop]) {
					allTokens.pop();
					Tree<string> Operator(top.literal);
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

void expressionNonBinary(stack<Token> &allTokens, Tree<string> &parentTree){ //logic for generation of an expressionNonBinary non-terminal

	stack<Token> usedTokens;
	queue<Tree<string> > usedTrees;

	for (int outerLoop = 0; outerLoop < EXPNB_TOKEN.size(); outerLoop++) {

		for (int innerLoop = 0; innerLoop < EXPNB_TOKEN[outerLoop].size();innerLoop++) {

			if (allTokens.size() > 0 && EXPNB_TOKEN[outerLoop][innerLoop] == EXPRESSIONB) {

				Token exprTokBi = {EXPRESSIONB, "EXPRESSIONB", 0,0};
				Tree<string> expressionBTree(exprTokBi.literal);
				expressionBinary(allTokens,expressionBTree);

				if (expressionBTree.numberOfChildren() > 0) {
					usedTrees.push(expressionBTree);
				}

                Token next = allTokens.top();

                if (innerLoop == EXPNB_TOKEN[outerLoop].size()-1) {

				while (usedTrees.size() > 0) {
					parentTree.addChild(usedTrees.front());
					usedTrees.pop();
				}

				return; //added because whenever an expressionBinary is expected, it's the end of the ExpressionNonBinary grammar

                }

			} else if (allTokens.size() > 0 && EXPNB_TOKEN[outerLoop][innerLoop] == EXPRESSIONNB) {

				Token exprTokNB = {EXPRESSIONNB, "EXPRESSIONNB", 0,0};
				Tree<string> expressionNBTree(exprTokNB.literal);
				expressionNonBinary(allTokens, expressionNBTree);
				usedTrees.push(expressionNBTree);

			} else if (allTokens.size() > 0 && EXPNB_TOKEN[outerLoop][innerLoop] == PARAMS) {

				if (allTokens.top().type != SEPARATOR_RIGHT_PAREN) {

					Token paramTok = {PARAMS,"PARAMS",0,0};
					Tree<string> paramTree(paramTok.literal);
					expressionNonBinary(allTokens,paramTree);

					while (allTokens.top().type != SEPARATOR_RIGHT_PAREN) {
						Token top = allTokens.top();

						if (top.type == OPERATOR_COMMA) {
							usedTokens.push(top);
							allTokens.pop();
							expressionNonBinary(allTokens,paramTree);

						} else {
							string error = "Failure at character " +top.literal+", row " + to_string(top.row) + ", column "+ to_string(top.column) + ". Expected a comma or right parenthesis, I did. This is why you fail \n";
							throw string(error);
						}

					}

					usedTrees.push(paramTree);
					Token top = allTokens.top();
					usedTokens.push(top);
                    Tree<string> rightParenTree(top.literal);
                    usedTrees.push(rightParenTree);
					allTokens.pop();

					while (usedTrees.size() > 0) { //added because right parenthesis is always at end of ExpNB grammar
						parentTree.addChild(usedTrees.front());
						usedTrees.pop();
					}

					return;

				}  else {

					Token top = allTokens.top();
					usedTokens.push(top);
                    Tree<string> rightParenTree(top.literal);
                    usedTrees.push(rightParenTree);
					allTokens.pop();

					while (usedTrees.size() > 0) {
						parentTree.addChild(usedTrees.front());
						usedTrees.pop();
					}

					return;

				}
			} else {
				Token top = allTokens.top();

				if (top.type == EXPNB_TOKEN[outerLoop][innerLoop]) {
					Tree<string> temp(top.literal);
					usedTrees.push(temp);
					usedTokens.push(top);
					allTokens.pop();

					if (innerLoop == EXPNB_TOKEN[outerLoop].size()-1) {

						while (usedTrees.size() > 0) {
							parentTree.addChild(usedTrees.front());
							usedTrees.pop();
						}

						return;
					}

				} else {

					if (outerLoop == EXPNB_TOKEN.size()-1) {
						string error = "Failure at character " +top.literal+", row " + to_string(top.row) + ", column "+ to_string(top.column) +". Matches a non-binary expression, it does not. This is why you fail \n";
						throw string (error);

					} else {

						while (usedTrees.size() > 0) {
							usedTrees.pop();
						}

						break;
					}
				}
			}
		} //inner loop ends here

		while (usedTokens.size() != 0) {
			allTokens.push(usedTokens.top());
			usedTokens.pop();
		}

	} //outer loop ends here

	while (usedTrees.size() > 0) {
		parentTree.addChild(usedTrees.front());
		usedTrees.pop();
	}

	return;

}


/*
Statements->Keyword_Then . Statement* . Keyword_You_Must |
			Keyword_If . Seperator_Left_Paren . ExpressionNonBinary . ExpressionBinary . Seperator_Right_Paren . Statement |
			Keyword_Do . Statement . Keyword_While . Seperator_Left_Paren . ExpressionNonBinary. ExpressionBinary . Separtor_Right_Paren |
			Keyword_Transmit . ExpressionNonBinary . ExpressionBinary . Seperator_Semicolon |
			Keyword_Let . Identifier_Variable . Operator_Be . ExpressionNonBinary . ExpressionBinary |
			ExpressionNonBinary . ExpressionBinary . Seperator_Semicolon
*/
void statements(stack<Token> &allTokens,Tree<string> &parentTree){
	Token top;
	for (int outerLoop = 0; outerLoop < STATEMENT_TOKEN.size(); outerLoop++){
		queue<Tree<string> > usedTrees ;
		stack<Token> usedTokens;
		bool allPassed = true;
		for (int innerLoop = 0 ; innerLoop < STATEMENT_TOKEN[outerLoop].size(); innerLoop++){
			if (allTokens.size() != 0 && STATEMENT_TOKEN[outerLoop][innerLoop] == STATEMENT){
				if (STATEMENT_TOKEN[outerLoop][innerLoop] == STATEMENT){// a statment can have a statement as its child
					if (outerLoop == 0) { // if the outerloop is 0 the statement follows the format "then, statement*, youmust" which will only stop adding statements once the keyword you must is found
						while (allTokens.size() != 0 && allTokens.top().type != KEYWORD_YOU_MUST) { // once keyword youmust is found the while loop ends
							Token stateHolder = {STATEMENT, "STATEMENT" , 0,0}; // placeholder statement token
							Tree<string> statementConverter(stateHolder.literal);
							statements(allTokens, statementConverter); // call statements again
							if (statementConverter.numberOfChildren() > 0) { // if no statement was added dont add to the parent tree
								usedTrees.push(statementConverter) ;
							}
							if (allTokens.size() != 0 && allTokens.top().type == SEPARATOR_SEMICOLON){ // if it is a semi colon in the statement then is must be added to the used tokens
								usedTokens.push(allTokens.top());
								allTokens.pop();
							}
						}
						Tree<string> you_must_token(allTokens.top().literal); // you must must always follow so it can be added the the tree
						usedTrees.push(you_must_token);
						usedTokens.push(allTokens.top());
						allTokens.pop();
						break; // end this inner loop to eventually add to parent tree
					} else {// otherwise  it follows another format of statement and this can be added as a child
						Token stateHolder = {STATEMENT, "STATEMENT" , 0,0};

						Tree<string> statementConverter(stateHolder.literal);
						statements(allTokens, statementConverter);
						usedTrees.push(statementConverter) ;
					}
				}
			} else if (allTokens.size() != 0 && STATEMENT_TOKEN[outerLoop][innerLoop] == EXPRESSIONNB){ // a statement can have expression non binary in it
				Token exprTokNB = {EXPRESSIONNB, "EXPRESSIONNB", 0,0};
				Tree<string> exprNB(exprTokNB.literal);
				expressionNonBinary(allTokens, exprNB);
				usedTrees.push(exprNB) ;
			} else if (allTokens.size() != 0 && STATEMENT_TOKEN[outerLoop][innerLoop] == EXPRESSIONB) {// a statemnet can have a expression binary in it
				Token exprTokB = {EXPRESSIONB, "EXPRESSIONB",0,0};
				Tree<string> exprB(exprTokB.literal);
				expressionBinary(allTokens,exprB);
				if (exprB.numberOfChildren() > 0) { // this can be empty, if so dont add to the tree
					usedTrees.push(exprB) ;
				}

			}
			else if (allTokens.size() != 0 && allTokens.top().type == STATEMENT_TOKEN[outerLoop][innerLoop]){ // General matching statement of tokens
				Token temp = allTokens.top();
				usedTokens.push(temp);
				Tree<string> temp2(temp.literal);
				usedTrees.push(temp2) ;
				allTokens.pop();
			}else{
				allPassed = false; // didnt match any tokens
				break;
			}

		}
		if (allPassed){ // if everything passes then add to the parent tree
			while (usedTrees.size() >0){
				parentTree.addChild(usedTrees.front());
				usedTrees.pop();
			}
			return; // return to end function
		}else{// otherwise backtrack and add everything back to all tokens
			while (usedTokens.size() != 0) {
				allTokens.push(usedTokens.top());
				usedTokens.pop();
			}
		}
	}
	string error = "Failure at character " +allTokens.top().literal+", row " + to_string(allTokens.top().row) + ", column "+ to_string(allTokens.top().column) + ". Matches a statement, it does not. This is why you fail \n";
	throw string(error);
}
/*
Functions -> Keyword_Force . Identifier_Function . Seperator_Left_Paren . Idenifier_Variable* . Seperator_Right_Paren . Keyword_Then . Statement . Keyword_You_must
*/
void functions(stack<Token> &allTokens, Tree<string> &parentTree){
	Token top; // top used to hold the top token of the stack
	for (int outerLoop = 0; outerLoop < FUNCTION_TOKEN.size(); outerLoop++){// loops over the outer part of the FUNCTION_TOKEN vector
		queue<Tree<string> > usedTrees ; // holds trees which have been created, if the language passes the trees will be popped into the parent tree
		stack<Token> usedTokens; // holds used tokens, if the language fails it must be added back to the allTokens stack
		bool allPassed = true;// used to see if tokens follow correct order of langauge
		for (int innerLoop = 0 ; innerLoop < FUNCTION_TOKEN[outerLoop].size(); innerLoop++){
			if (FUNCTION_TOKEN[outerLoop][innerLoop] == IDENTIFIER_VARIABLE){
				while (allTokens.size() != 0 && allTokens.top().type == IDENTIFIER_VARIABLE){ // Since many identifier variables can come after eachother, it will go until there is one left (0..*)
					usedTokens.push(allTokens.top()); // adds token to used tokens
					Token temp = allTokens.top(); // creates token to add to new tree
					Tree<string> temp2(temp.literal);
					usedTrees.push(temp2) ; // adds newly made tree to used trees
					allTokens.pop();
					if (allTokens.size() != 0 && allTokens.top().type == OPERATOR_COMMA){ // if there is a comma there must be another identifier so pop off the comma
						usedTokens.push(allTokens.top());
						allTokens.pop();
					}// comma is not added to the tree
				}
				continue; // Dont do anything else in this innerloop, go to innerloop +1
			}else if (allTokens.size() != 0 && FUNCTION_TOKEN[outerLoop][innerLoop] == STATEMENT){
				while ((allTokens.top().type != KEYWORD_YOU_MUST) || (allTokens.top().type != KEYWORD_FORCE)){ // statements can be after statements inside a functions, unless the word youmust of force is found it is another statement
					Token stateHolder = {STATEMENT, "STATEMENT" , 0,0}; // create a statement placeholder token
					if (allTokens.size() != 0 && allTokens.top().type == KEYWORD_YOU_MUST){ // you must will end the while loop
						break;
					}
					Tree<string> statementConverter(stateHolder.literal); // create the statement tree for the next set of tokens
					statements(allTokens, statementConverter);
					if (statementConverter.numberOfChildren() > 0) { // statement tree could be empty, if so dont add it to the tree
						usedTrees.push(statementConverter) ;
					}
				}
			}else if (allTokens.size() != 0 && allTokens.top().type == FUNCTION_TOKEN[outerLoop][innerLoop]){ // normal token match, adding it to the used tree and used tokens
				Token temp = allTokens.top();
				Tree<string> temp2(temp.literal);
				usedTrees.push(temp2) ;
				usedTokens.push(temp);
				allTokens.pop();
			}else{
				allPassed = false; // no match found, break from inner loop
				break;
			}
		}
		if (allPassed){ // everything was correct then add to the parent tree and return (ends the function)
			while (usedTrees.size() >0){
				parentTree.addChild(usedTrees.front());
				usedTrees.pop();
			}
			return;
		}else{ // if not add all the tokens back onto the allTokens stack
			while (usedTokens.size() != 0) {
				allTokens.push(usedTokens.top());
				usedTokens.pop();
			}
		}
	}// If it gets out of outerloop, if has checked all the tokens agaisnt the the function part of the language and therefore it must not match something at the top value of the allTokens stack
	string error = "Failure at character " +allTokens.top().literal+", row " + to_string(allTokens.top().row) + ", column "+ to_string(allTokens.top().column) + ". Matches a function, it does not. This is why you fail. \n";
	throw string(error);

}
/*
lib -> KEYWORD_PRETEXT . Function* . KEYWORD_EOF
*/
void lib(stack<Token> &allTokens, Tree<string> &parentTree)   { //checks for initial token, then checks for functions until end-token is found

	queue<Tree<string> > usedTrees;

	if ( allTokens.size() < 0 || allTokens.top().type != KEYWORD_PRETEXT){
        string error = "Expected the Pretext Keyword, I did. This is why you fail \n";
    	throw string(error);
    }

	Token top = allTokens.top();
	Tree<string> libTree(top.literal);
	parentTree.addChild(libTree);
	allTokens.pop();
	
	while (allTokens.size() > 0 && allTokens.top().type != KEYWORD_THE_END ) {
		Token FuncTok = {FUNCTIONS,"FUNCTION",0,0};
		Tree<string> functionTree(FuncTok.literal);
		functions(allTokens, functionTree);
		usedTrees.push(functionTree);
	}

	if (allTokens.size() > 0 && allTokens.top().type == KEYWORD_THE_END) {
		Token top = allTokens.top();
		allTokens.pop();
		Tree<string> end(top.literal);
		usedTrees.push(end);

	} else { //if not, indicate the error
		string error = "Failure. Expected another function, or the End of File keyword, I did. This is why you fail. \n";
		throw string(error);
	}

	while (usedTrees.size() > 0) { //adding all the functions to the parent tree
		parentTree.addChild(usedTrees.front());
		usedTrees.pop();
	}

}


/*
ExpNB -> IDENTIFIER_VARIABLE . ExpB |
		 LITERAL_INTEGER . ExpB |
		 LITERAL_STRING . ExpB |
		 LITERAL_CHAR . ExpB |
		 SEPARATOR_LEFT_PAREN . ExpNB . SEPARATOR_RIGHT_PAREN |
		 KEYWORD_EXECUTE . IDENTIFIER_FUNCTION . SEPARATOR_LEFT_PAREN . Params . SEPARATOR_RIGHT_PAREN |
*/



