#include <string>
#include<iostream>
#include "tree.h"
#include "tokenType.h"
#include <stack>
#include <vector>
#include <queue>
using namespace std;

class Parser{
public:

	const vector<vector<TokenType> > FUNCTIONTOKEN = { {TokenType::KEYWORD_FORCE, TokenType::IDENTIFIER_FUNCTION, TokenType::SEPARATOR_LEFT_PAREN, TokenType::IDENTIFIER_VARIABLE, TokenType::SEPARATOR_RIGHT_PAREN, TokenType::KEYWORD_THEN, TokenType::STATEMENT ,TokenType::KEYWORD_YOU_MUST} };
	const vector<vector<TokenType> > STATEMENTOKEN = { { TokenType::KEYWORD_THEN , TokenType::STATEMENT , TokenType::KEYWORD_YOU_MUST},
												  {TokenType::KEYWORD_IF , TokenType::SEPARATOR_LEFT_PAREN , TokenType::EXPRESSIONNB , TokenType::EXPRESSIONB , TokenType::SEPARATOR_RIGHT_PAREN , TokenType::STATEMENT },
												  {TokenType::KEYWORD_DO , TokenType::STATEMENT , TokenType::KEYWORD_WHILE , TokenType::SEPARATOR_LEFT_PAREN , TokenType::EXPRESSIONNB , TokenType::EXPRESSIONB , TokenType::SEPARATOR_RIGHT_PAREN },
												  {TokenType::KEYWORD_TRANSMIT , TokenType::EXPRESSIONNB , TokenType::EXPRESSIONB , TokenType::SEPARATOR_SEMICOLON },
												  {TokenType::KEYWORD_LET , TokenType::IDENTIFIER_VARIABLE , TokenType::OPERATOR_BE , TokenType::EXPRESSIONNB , TokenType::EXPRESSIONB, TokenType::SEPARATOR_SEMICOLON },
												  {TokenType::EXPRESSIONNB , TokenType::EXPRESSIONB , TokenType::SEPARATOR_SEMICOLON}};
	const vector<vector<TokenType> > LIBTOKEN = { {TokenType::KEYWORD_PRETEXT, TokenType::FUNCTIONS, TokenType::KEYWORD_THE_END}};
	const vector<vector<TokenType> > EXPBTOKEN = { {TokenType::OPERATOR_PLUS, TokenType::EXPRESSIONNB},
	 										  {TokenType::OPERATOR_TIMES, TokenType::EXPRESSIONNB},
											  {TokenType::OPERATOR_MINUS, TokenType::EXPRESSIONNB},
											  {TokenType::OPERATOR_DIVIDE, TokenType::EXPRESSIONNB},
											  {TokenType::OPERATOR_MOD, TokenType::EXPRESSIONNB},
											  {TokenType::OPERATOR_POW, TokenType::EXPRESSIONNB},
											  {TokenType::OPERATOR_OR, TokenType::EXPRESSIONNB},
											  {TokenType::OPERATOR_AND, TokenType::EXPRESSIONNB},
											  {TokenType::OPERATOR_IS, TokenType::EXPRESSIONNB},
											  {TokenType::OPERATOR_GT, TokenType::EXPRESSIONNB},
											  {TokenType::OPERATOR_GTE, TokenType::EXPRESSIONNB},
											  {TokenType::OPERATOR_LT, TokenType::EXPRESSIONNB},
											  {TokenType::OPERATOR_LTE, TokenType::EXPRESSIONNB},
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
void lib(stack<Token> &allTokens, Tree<string> &parentTree)   {

	queue<Tree<string> > usedTrees;
	if (allTokens.top().type != TokenType::KEYWORD_PRETEXT){ //checking that the the KEYWORD_PRETEXT is present
        string error = "Expected the Pretext Keyword, I did. This is why you fail \n";
    	throw string(error);
    }
	Token top = allTokens.top();
	Tree<string> libTree(top.literal);
	parentTree.addChild(libTree);
	allTokens.pop();
	top = allTokens.top();
	while (allTokens.size() > 0 && allTokens.top().type != TokenType::KEYWORD_THE_END ) { //while it's not the end of the file, check for functions
		Token FuncTok = {TokenType::FUNCTIONS,"FUNCTION",0,0};
		Tree<string> functionTree(FuncTok.literal); //make functionTree
		functions(allTokens, functionTree); //send functionTree to the Functions function
		usedTrees.push(functionTree);
	}

	if (allTokens.top().type == TokenType::KEYWORD_THE_END) { //checks if the keyword indicating the end of file is present
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
Functions -> Keyword_Force . Identifier_Function . Seperator_Left_Paren . Idenifier_Variable* . Seperator_Right_Paren . Keyword_Then . Statement . Keyword_You_must
*/
void functions(stack<Token> &allTokens, Tree<string> &parentTree){
	Token top; // top used to hold the top token of the stack 
	for (int outerLoop = 0; outerLoop < FUNCTIONTOKEN.size(); outerLoop++){// loops over the outer part of the FUNCTIONTOKEN vector
		queue<Tree<string> > usedTrees ; // holds trees which have been created, if the language passes the trees will be popped into the parent tree
		stack<Token> usedTokens; // holds used tokens, if the language fails it must be added back to the allTokens stack
		bool allPassed = true;// used to see if tokens follow correct order of langauge
		for (int innerLoop = 0 ; innerLoop < FUNCTIONTOKEN[outerLoop].size(); innerLoop++){
			if (FUNCTIONTOKEN[outerLoop][innerLoop] == TokenType::IDENTIFIER_VARIABLE){ 
				while (allTokens.size() != 0 && allTokens.top().type == TokenType::IDENTIFIER_VARIABLE){ // Since many identifier variables can come after eachother, it will go until there is one left (0..*)
					usedTokens.push(allTokens.top()); // adds token to used tokens
					Token temp = allTokens.top(); // creates token to add to new tree
					Tree<string> temp2(temp.literal);
					usedTrees.push(temp2) ; // adds newly made tree to used trees
					allTokens.pop(); 
					if (allTokens.top().type == TokenType::OPERATOR_COMMA){ // if there is a comma there must be another identifier so pop off the comma
						usedTokens.push(allTokens.top());
						allTokens.pop();
					}// comma is not added to the tree
				}
				continue; // Dont do anything else in this innerloop, go to innerloop +1
			}else if (allTokens.size() != 0 && FUNCTIONTOKEN[outerLoop][innerLoop] == TokenType::STATEMENT){
				while ((allTokens.top().type != TokenType::KEYWORD_YOU_MUST) || (allTokens.top().type != TokenType::KEYWORD_FORCE)){ // statements can be after statements inside a functions, unless the word youmust of force is found it is another statement
					Token stateHolder = {TokenType::STATEMENT, "STATEMENT" , 0,0}; // create a statement placeholder token
					if (allTokens.top().type == TokenType::KEYWORD_YOU_MUST){ // you must will end the while loop
						break;
					}
					Tree<string> statementConverter(stateHolder.literal); // create the statement tree for the next set of tokens
					statements(allTokens, statementConverter);
					if (statementConverter.numberOfChildren() > 0) { // statement tree could be empty, if so dont add it to the tree
						usedTrees.push(statementConverter) ;
					}
				}
			}else if (allTokens.top().type == FUNCTIONTOKEN[outerLoop][innerLoop]){ // normal token match, adding it to the used tree and used tokens
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
Statements->Keyword_Then . Statement* . Keyword_You_Must |
			Keyword_If . Seperator_Left_Paren . ExpressionNonBinary . ExpressionBinary . Seperator_Right_Paren . Statement |
			Keyword_Do . Statement . Keyword_While . Seperator_Left_Paren . ExpressionNonBinary. ExpressionBinary . Separtor_Right_Paren |
			Keyword_Transmit . ExpressionNonBinary . ExpressionBinary . Seperator_Semicolon |
			Keyword_Let . Identifier_Variable . Operator_Be . ExpressionNonBinary . ExpressionBinary |
			ExpressionNonBinary . ExpressionBinary . Seperator_Semicolon
*/
void statements(stack<Token> &allTokens,Tree<string> &parentTree){
	Token top;
	for (int outerLoop = 0; outerLoop < STATEMENTOKEN.size(); outerLoop++){
		queue<Tree<string> > usedTrees ;
		stack<Token> usedTokens;
		bool allPassed = true;
		for (int innerLoop = 0 ; innerLoop < STATEMENTOKEN[outerLoop].size(); innerLoop++){
			if (allTokens.size() != 0 && STATEMENTOKEN[outerLoop][innerLoop] == TokenType::STATEMENT){
				if (STATEMENTOKEN[outerLoop][innerLoop] == TokenType::STATEMENT){// a statment can have a statement as its child
					if (outerLoop == 0) { // if the outerloop is 0 the statement follows the format "then, statement*, youmust" which will only stop adding statements once the keyword you must is found
						while (allTokens.size() != 0 && allTokens.top().type != TokenType::KEYWORD_YOU_MUST) { // once keyword youmust is found the while loop ends
							Token stateHolder = {TokenType::STATEMENT, "STATEMENT" , 0,0}; // placeholder statement token
							Tree<string> statementConverter(stateHolder.literal);
							statements(allTokens, statementConverter); // call statements again 
							if (statementConverter.numberOfChildren() > 0) { // if no statement was added dont add to the parent tree
								usedTrees.push(statementConverter) ;
							}
							if (allTokens.top().type == TokenType::SEPARATOR_SEMICOLON){ // if it is a semi colon in the statement then is must be added to the used tokens
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
						Token stateHolder = {TokenType::STATEMENT, "STATEMENT" , 0,0};

						Tree<string> statementConverter(stateHolder.literal);
						statements(allTokens, statementConverter);
						usedTrees.push(statementConverter) ;
					}
				}
			} else if (allTokens.size() != 0 && STATEMENTOKEN[outerLoop][innerLoop] == TokenType::EXPRESSIONNB){ // a statement can have expression non binary in it
				Token exprTokNB = {TokenType::EXPRESSIONNB, "EXPRESSIONNB", 0,0};
				Tree<string> exprNB(exprTokNB.literal);
				expressionNonBinary(allTokens, exprNB);
				usedTrees.push(exprNB) ;
			} else if (allTokens.size() != 0 && STATEMENTOKEN[outerLoop][innerLoop] == TokenType::EXPRESSIONB) {// a statemnet can have a expression binary in it
				Token exprTokB = {TokenType::EXPRESSIONB, "EXPRESSIONB",0,0};
				Tree<string> exprB(exprTokB.literal);
				expressionBinary(allTokens,exprB);
				if (exprB.numberOfChildren() > 0) { // this can be empty, if so dont add to the tree
					usedTrees.push(exprB) ;
				}

			}
			else if (allTokens.size() != 0 && allTokens.top().type == STATEMENTOKEN[outerLoop][innerLoop]){ // General matching statement of tokens
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
void expressionBinary(stack<Token> &allTokens, Tree<string> &parentTree){
	queue<Tree<string> > usedTrees;
	for (int outerLoop = 0; outerLoop < EXPBTOKEN.size(); outerLoop++) {
		for (int innerLoop = 0; innerLoop < EXPBTOKEN[outerLoop].size(); innerLoop++) {
			if (allTokens.size() > 0 && EXPBTOKEN[outerLoop][innerLoop] == TokenType::EXPRESSIONNB) { //checks if a Non-Binary Expression is expected
				Token exprTokNB = {TokenType::EXPRESSIONNB, "EXPRESSIONNB", 0,0};
				Tree<string> expressionNBTree(exprTokNB.literal);
				expressionNonBinary(allTokens, expressionNBTree); //sending the new tree to expressionNonBinary to add children to it
				usedTrees.push(expressionNBTree); //add the new tree to usedTrees
				while (usedTrees.size() > 0) {
					parentTree.addChild(usedTrees.front());
					usedTrees.pop();
				}
				return;  //the while loop above + the return statement was added here because ExpressionNonBinary is always at the end of the ExpressionBinary grammar
			} else {
				Token top = allTokens.top();
				if (top.type == EXPBTOKEN[outerLoop][innerLoop]) { //check if token matches any of the given operators
					allTokens.pop();
					Tree<string> Operator(top.literal); //if it matches, pop the token, create a string Tree from it, and add the tree to usedTrees
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
void expressionNonBinary(stack<Token> &allTokens, Tree<string> &parentTree){
	stack<Token> usedTokens;
	queue<Tree<string> > usedTrees;
	for (int outerLoop = 0; outerLoop < EXPNBTOKEN.size(); outerLoop++) {
		for (int innerLoop = 0; innerLoop < EXPNBTOKEN[outerLoop].size();innerLoop++) {
			if (allTokens.size() > 0 && EXPNBTOKEN[outerLoop][innerLoop] == TokenType::EXPRESSIONB) { //checking if a Binary expression is expected
				Token exprTokBi = {TokenType::EXPRESSIONB, "EXPRESSIONB", 0,0};
				Tree<string> expressionBTree(exprTokBi.literal);
				expressionBinary(allTokens,expressionBTree); //sending the new tree to expressionBinary to add children to it
				if (expressionBTree.numberOfChildren() > 0) { //if the returned expressionBinary is not empty, add the tree to usedTrees
					usedTrees.push(expressionBTree);
				}
                Token next = allTokens.top();
                if (innerLoop == EXPNBTOKEN[outerLoop].size()-1) { //checking whether the end of this EXPNB has been reached
				while (usedTrees.size() > 0) {
					parentTree.addChild(usedTrees.front());
					usedTrees.pop();
				}
				return;
                }
			} else if (allTokens.size() > 0 && EXPNBTOKEN[outerLoop][innerLoop] == TokenType::EXPRESSIONNB) { //checking if a NonBinary expression is expected
				Token exprTokNB = {TokenType::EXPRESSIONNB, "EXPRESSIONNB", 0,0};
				Tree<string> expressionNBTree(exprTokNB.literal);
				expressionNonBinary(allTokens, expressionNBTree);
				usedTrees.push(expressionNBTree);
			} else if (allTokens.size() > 0 && EXPNBTOKEN[outerLoop][innerLoop] == TokenType::PARAMS) { //checking if parameters are expected
				if (allTokens.top().type != TokenType::SEPARATOR_RIGHT_PAREN) { //following lines implement logic to check for 0, 1 or more parameters.
					Token paramTok = {TokenType::PARAMS,"PARAMS",0,0};
					Tree<string> paramTree(paramTok.literal);
					expressionNonBinary(allTokens,paramTree);
					while (allTokens.top().type != TokenType::SEPARATOR_RIGHT_PAREN) {
						Token top = allTokens.top();
						if (top.type == TokenType::OPERATOR_COMMA) {
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
					while (usedTrees.size() > 0) { //added because right parenthesis is always at end of ExpNB
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
					while (usedTrees.size() > 0) { //added because right parenthesis is always at end of ExpNB
						parentTree.addChild(usedTrees.front());
						usedTrees.pop();
					}
					return;

				}
			} else {
				Token top = allTokens.top();
				if (top.type == EXPNBTOKEN[outerLoop][innerLoop]) { //matching terminals
					Tree<string> temp(top.literal);
					usedTrees.push(temp);
					usedTokens.push(top);
					allTokens.pop();
					if (innerLoop == EXPNBTOKEN[outerLoop].size()-1) {//checking whether the end of the grammar has been reached
						while (usedTrees.size() > 0) {
							parentTree.addChild(usedTrees.front());
							usedTrees.pop();
						}
						return;
					}
				} else { //if it doesn't match
					if (outerLoop == EXPNBTOKEN.size()-1) { //if this was the final grammar option, then nothing matches and throw an error
						string error = "Failure at character " +top.literal+", row " + to_string(top.row) + ", column "+ to_string(top.column) +". Matches a non-binary expression, it does not. This is why you fail \n";
						throw string (error);
					} else { //if it wasn't the final grammar option, empty usedTrees and break the inner loop, effectively moving on to the next grammar option
						while (usedTrees.size() > 0) {
							usedTrees.pop();
						}
						break;
					}
				}
			}
		} //inner loop ends here
		while (usedTokens.size() != 0) { //once inner loop ends, if it hasn't matched, deposit all the usedTokens onto the main stack again
			allTokens.push(usedTokens.top());
			usedTokens.pop();
		}
	} //outer loop ends here
	while (usedTrees.size() > 0) { //adding the matched children to the parent tree
		parentTree.addChild(usedTrees.front());
		usedTrees.pop();
	}
	return;

    }


};
