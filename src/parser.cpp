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
												  {TokenType::KEYWORD_IF , TokenType::SEPARATOR_LEFT_PAREN , TokenType::EXPRESSIONNB , TokenType::EXPRESSIONB , TokenType::SEPARATOR_RIGHT_PAREN , TokenType::STATEMENT },
												  {TokenType::KEYWORD_DO , TokenType::STATEMENT , TokenType::KEYWORD_WHILE , TokenType::SEPARATOR_LEFT_PAREN , TokenType::EXPRESSIONNB , TokenType::EXPRESSIONB , TokenType::SEPARATOR_RIGHT_PAREN },
												  {TokenType::KEYWORD_TRANSMIT , TokenType::EXPRESSIONNB , TokenType::EXPRESSIONB , TokenType::SEPARATOR_SEMICOLON },
												  {TokenType::KEYWORD_LET , TokenType::IDENTIFIER_VARIABLE , TokenType::OPERATOR_BE , TokenType::EXPRESSIONNB , TokenType::EXPRESSIONB, TokenType::SEPARATOR_SEMICOLON },
												  {TokenType::EXPRESSIONNB , TokenType::EXPRESSIONB , TokenType::SEPARATOR_SEMICOLON}};
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
	Token top = allTokens.top();
	cout << "got into the lib" << endl;
	while (allTokens.size() > 0 && allTokens.top().type != TokenType::KEYWORD_THE_END ) {
		Token FuncTok = {TokenType::FUNCTIONS,"FUNCTION",0,0};
		cout << "added function to tree" << endl;
		Tree<string> functionTree(FuncTok.literal);//example of token constructor. row + column doesnt matter in this case because it's just a keyword we're adding
		cout << "going into function call" << endl;
		functions(allTokens, functionTree);
		usedTrees.push(functionTree);
		cout << "Ended function in tree " << endl;
	}

	if (allTokens.top().type == TokenType::KEYWORD_THE_END) {
		cout << "Got to type the end" << endl;
		Token top = allTokens.top();
		allTokens.pop();
		Tree<string> end(top.literal);
		usedTrees.push(end);
	} else {
		string error = "Failure at character " +allTokens.top().literal+", row " + to_string(allTokens.top().row) + ", column "+ to_string(allTokens.top().column) + ". Expected another function, or the End of File keyword, I did. This is why you fail. \n";
		throw string(error);
	}

	while (usedTrees.size() > 0) {
		parentTree.addChild(usedTrees.front());
		usedTrees.pop();
	}
}


/*
Functions -> Keyword_Force . Identifier_Function . Seperator_Left_Paren . Idenifier_Variable* . Seperator_Right_Paren . Keyword_Then . Statement . Keyword_You_must
*/
	void functions(stack<Token> &allTokens, Tree<string> &parentTree){
		Token top;
		cout << "FUNCTION : Function is called" << endl;
		for (int outerLoop = 0; outerLoop < FUNCTIONTOKEN.size(); outerLoop++){
			queue<Tree<string> > usedTrees ;
			stack<Token> usedTokens;
			bool allPassed = true;
			cout << "FUNCTION : outerloop called " << outerLoop << endl;
			for (int innerLoop = 0 ; innerLoop < FUNCTIONTOKEN[outerLoop].size(); innerLoop++){
				cout << "FUNCTION : innerloop called " << innerLoop << endl;
				cout << "top token, row, column = " << "'" << allTokens.top().literal << "'" << ", " << allTokens.top().row << ", " << allTokens.top().column << endl;
				if (FUNCTIONTOKEN[outerLoop][innerLoop] == TokenType::IDENTIFIER_VARIABLE){
					while (allTokens.size() != 0 && allTokens.top().type == TokenType::IDENTIFIER_VARIABLE){
						//funcTree.AddChild(Tree<string>(allTokens.top()));
						usedTokens.push(allTokens.top());
						Token temp = allTokens.top();
						Tree<string> temp2(temp.literal);
						usedTrees.push(temp2) ;
						allTokens.pop();
						if (allTokens.top().type == TokenType::OPERATOR_COMMA){
							usedTokens.push(allTokens.top());
							allTokens.pop();
						}
					}
					continue;
				}else if (allTokens.size() != 0 && FUNCTIONTOKEN[outerLoop][innerLoop] == TokenType::STATEMENT){
					//Tree<string> tempTree = Statements(allTokens);
					//funcTree.AddChild(tempTree);
					//usedTokens.push(allTokens.top());
					//
					//TODO Here is where we have to add the placeholder statement call
					//
					while ((allTokens.top().type != TokenType::KEYWORD_YOU_MUST) || (allTokens.top().type != TokenType::KEYWORD_FORCE)){
						Token stateHolder = {TokenType::STATEMENT, "STATEMENT" , 0,0};
						cout << "top token, row, column = " << "'" << allTokens.top().literal << endl;
						if (allTokens.top().type == TokenType::KEYWORD_YOU_MUST){
							break;
						}
						cout << "FUNCTION : STATEMENT called "  << endl;
						Tree<string> statementConverter(stateHolder.literal);
						//allTokens.pop();
						statements(allTokens, statementConverter);
						if (statementConverter.numberOfChildren() > 0) {
							usedTrees.push(statementConverter) ;
						}
					}
				}else if (allTokens.top().type == FUNCTIONTOKEN[outerLoop][innerLoop]){
					//check to see how this works
					cout << "FUNCTION : Macthed token " << allTokens.top().literal  << endl;
					Token temp = allTokens.top();
					Tree<string> temp2(temp.literal);
					usedTrees.push(temp2) ;
					usedTokens.push(temp);
					allTokens.pop();
					//top = allTokens.top();
					//funcTree.AddChild(top);
				}else{
					allPassed = false;
					break;
				}
			//	if (allTokens.size() != 0){ //@Dylan: I commented this out and added the pop on line 191, because it was popping the "you_must" off the stack after completing a statement, but before the next loop could match with it
		//			allTokens.pop();
			//	}
			}
			if (allPassed){
				while (usedTrees.size() >0){
					parentTree.addChild(usedTrees.front());
					usedTrees.pop();
				}
				return;
			}else{
				while (usedTokens.size() != 0) {
					allTokens.push(usedTokens.top());
					usedTokens.pop();
				}
			}
			//Add correction to see if it fails.
		}
		/*throw string("Failure at character %s, row %d, column %d.
							This is why you fail",
							top.literal, top.row, top.column);*/
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
	cout << "STATEMENT : statement called " << endl;
	for (int outerLoop = 0; outerLoop < STATEMENTOKEN.size(); outerLoop++){
		cout << "STATEMENT : outerloop called " << outerLoop << endl;
		queue<Tree<string> > usedTrees ;
		stack<Token> usedTokens;
		bool allPassed = true;
		for (int innerLoop = 0 ; innerLoop < STATEMENTOKEN[outerLoop].size(); innerLoop++){
			cout << "STATEMENT : innerloop called " << innerLoop << endl;
			cout << "top token, row, column = " << "'" << allTokens.top().literal << "'" << ", " << allTokens.top().row << ", " << allTokens.top().column << endl;
			if (allTokens.size() != 0 && STATEMENTOKEN[outerLoop][innerLoop] == TokenType::STATEMENT){
				//Tree<string> tempTree = Statements(allTokens);
				//funcTree.AddChild(tempTree);
				cout << "STATEMENT : ADDING STATEMENT " << endl;
				if (STATEMENTOKEN[outerLoop][innerLoop] == TokenType::STATEMENT){
					/*while (allTokens.size() != 0 && allTokens.top().type == TokenType::STATEMENT){
						Token stateHolder = {TokenType::STATEMENT, "STATEMENT" , 0,0};

						Tree<string> statementConverter(stateHolder.literal);
						//allTokens.pop();
						statements(allTokens, statementConverter);
						usedTrees.push(statementConverter) ;

						if (allTokens.top().type == TokenType::OPERATOR_COMMA){
							usedTokens.push(allTokens.top());
							allTokens.pop();
						}
					} */
					//continue;


					if (outerLoop == 0) {
						while (allTokens.size() != 0 && allTokens.top().type != TokenType::KEYWORD_YOU_MUST) {
							Token stateHolder = {TokenType::STATEMENT, "STATEMENT" , 0,0};
							cout << "STATEMENT : Into you must statement" << endl;
							Tree<string> statementConverter(stateHolder.literal);
							//allTokens.pop();
							statements(allTokens, statementConverter);
							if (statementConverter.numberOfChildren() > 0) {
								cout << "STATEMENT : It has children" << endl;
								usedTrees.push(statementConverter) ;
							}
							if (allTokens.top().type == TokenType::SEPARATOR_SEMICOLON){
								usedTokens.push(allTokens.top());
								allTokens.pop();
							}
						}
						Tree<string> you_must_token(allTokens.top().literal);
						usedTrees.push(you_must_token);
						usedTokens.push(allTokens.top());
						allTokens.pop();
						break;
					} else {
						Token stateHolder = {TokenType::STATEMENT, "STATEMENT" , 0,0};

						Tree<string> statementConverter(stateHolder.literal);
						//allTokens.pop();
						statements(allTokens, statementConverter);
						usedTrees.push(statementConverter) ;
					}
				}
				// if (outerLoop == 0){
				// 	while (allTokens.size() != 0 && allTokens.top().type != TokenType::KEYWORD_YOU_MUST) {
				// 	}
				// }else{

				// }


		//Add correction to see if it fails.
			}else if (allTokens.size() != 0 && STATEMENTOKEN[outerLoop][innerLoop] == TokenType::EXPRESSIONNB){
					//usedTokens.push(allTokens.top());
					//
					//TODO Here is where we have to add the placeholder expression call
					//
				Token exprTokNB = {TokenType::EXPRESSIONNB, "EXPRESSIONNB", 0,0};
				cout << "STATEMENT : Calling expression NonBinary " << endl;
				Tree<string> exprNB(exprTokNB.literal);
				//allTokens.pop();
				expressionNonBinary(allTokens, exprNB);
				cout << "STATEMENT : expression NonBinary made " << endl;
				usedTrees.push(exprNB) ;
			} else if (allTokens.size() != 0 && STATEMENTOKEN[outerLoop][innerLoop] == TokenType::EXPRESSIONB) {
				Token exprTokB = {TokenType::EXPRESSIONB, "EXPRESSIONB",0,0};
				cout << "STATEMENT : Calling expression Binary " << endl;
				Tree<string> exprB(exprTokB.literal);
				expressionBinary(allTokens,exprB);
				cout << "STATEMENT : expression Binary made " << endl;
				if (exprB.numberOfChildren() > 0) {
					usedTrees.push(exprB) ;
				}

			}
			else if (allTokens.size() != 0 && allTokens.top().type == STATEMENTOKEN[outerLoop][innerLoop]){
				//check to see how this works
				cout << "STATEMENT : Matched token " << endl;
				Token temp = allTokens.top();
				usedTokens.push(temp);
				Tree<string> temp2(temp.literal);
				usedTrees.push(temp2) ;
				allTokens.pop();
				//top = allTokens.top();
				//funcTree.AddChild(top);
			}else{
				cout << "STATEMENT : Unmatched Token " << endl;
				allPassed = false;
				break;
			}
			cout << "STATEMENT : Inner loop ran "  << endl;
			if (allTokens.size()  ==0){
				allTokens.pop();
			}
		}
		cout << "STATEMENT : Exit innerloop " << endl;
		cout << "top token, row, column = " << "'" << allTokens.top().literal << "'" << ", " << allTokens.top().row << ", " << allTokens.top().column << endl;
		if (allPassed){
			while (usedTrees.size() >0){
				parentTree.addChild(usedTrees.front());
				usedTrees.pop();
			}
			return;
		}else{
			cout << "STATEMENT : BACKTRACKING " << endl;
			while (usedTokens.size() != 0) {
				cout << "STATEMENT : RETURNING " << usedTokens.top().literal << endl;
				allTokens.push(usedTokens.top());
				usedTokens.pop();
			}
		}
	/*throw string("Failure at character %s, row %d, column %d.
						This is why you fail",
						top.literal, top.row, top.column);*/


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
		cout << "EXPRESSION BINARY: Outerloop " << outerLoop << endl;
		for (int innerLoop = 0; innerLoop < EXPBTOKEN[outerLoop].size(); innerLoop++) {
			cout << "EXPRESSION BINARY: innerLoop " << innerLoop << endl;
			if (allTokens.size() > 0 && EXPBTOKEN[outerLoop][innerLoop] == TokenType::EXPRESSIONNB) {
				Token exprTokNB = {TokenType::EXPRESSIONNB, "EXPRESSIONNB", 0,0};
				Tree<string> expressionNBTree(exprTokNB.literal);
				expressionNonBinary(allTokens, expressionNBTree);
				usedTrees.push(expressionNBTree);
				// commented out because what if things dont match down stream and then its on the parent
				while (usedTrees.size() > 0) { //because expB always ends on ExpNB
					parentTree.addChild(usedTrees.front());
					usedTrees.pop();
				}
				return;
			} else {
				Token top = allTokens.top();
				if (top.type == EXPBTOKEN[outerLoop][innerLoop]) {
					cout << "EXPRESSION BINARY: matched token in EXPB" << endl;
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
	cout << "EXPRESSION NONBINARY : expression start " << endl;
	for (int outerLoop = 0; outerLoop < EXPNBTOKEN.size(); outerLoop++) {
		cout << "EXPRESSION NONBINARY : outerloop called " << outerLoop << endl;
		for (int innerLoop = 0; innerLoop < EXPNBTOKEN[outerLoop].size();innerLoop++) {
			cout << "EXPRESSION NONBINARY : 		innerloop called " << innerLoop << endl;
			cout << "top token, row, column = " << "'" << allTokens.top().literal << "'" << ", " << allTokens.top().row << ", " << allTokens.top().column << endl;
			if (allTokens.size() > 0 && EXPNBTOKEN[outerLoop][innerLoop] == TokenType::EXPRESSIONB) {
                cout << "Into EXPB check" << endl;
				Token exprTokBi = {TokenType::EXPRESSIONB, "EXPRESSIONB", 0,0};
				Tree<string> expressionBTree(exprTokBi.literal);
				expressionBinary(allTokens,expressionBTree);
				if (expressionBTree.numberOfChildren() > 0) {
					usedTrees.push(expressionBTree);
				}
                cout << "Done with EXPB" << endl;
                Token next = allTokens.top();
                if (innerLoop == EXPNBTOKEN[outerLoop].size()-1) { //meaning that the end of this EXNB has been reached
				while (usedTrees.size() > 0) {
					parentTree.addChild(usedTrees.front());
					usedTrees.pop();
				}
				return;
                }
			} else if (allTokens.size() > 0 && EXPNBTOKEN[outerLoop][innerLoop] == TokenType::EXPRESSIONNB) {
				cout << "Into EXPNB check" << endl;
				Token exprTokNB = {TokenType::EXPRESSIONNB, "EXPRESSIONNB", 0,0};
				Tree<string> expressionNBTree(exprTokNB.literal);
				expressionNonBinary(allTokens, expressionNBTree);
				usedTrees.push(expressionNBTree);
			} else if (allTokens.size() > 0 && EXPNBTOKEN[outerLoop][innerLoop] == TokenType::PARAMS) { //checking parameters
                 cout << "Into PARAMS check" << endl;
				if (allTokens.top().type != TokenType::SEPARATOR_RIGHT_PAREN) {
					Token paramTok = {TokenType::PARAMS,"PARAMS",0,0};
					Tree<string> paramTree(paramTok.literal);
					expressionNonBinary(allTokens,paramTree);
                    cout << "out of ExpNB in PARAMS" << endl;
				//	usedTrees.push(paramTree);
					cout << "top token, row, column = " << "'" << allTokens.top().literal << "'" << ", " << allTokens.top().row << ", " << allTokens.top().column << endl;
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
					cout << "Got past params right" << endl;
					Token top = allTokens.top();
					usedTokens.push(top);
                    Tree<string> rightParenTree(top.literal);
                    usedTrees.push(rightParenTree);
					allTokens.pop();
					while (usedTrees.size() > 0) { //because right paren is always at end of ExpNB
						parentTree.addChild(usedTrees.front());
						usedTrees.pop();
					}
					return;
				}  else {
					cout << "Reading it the wrong way " << endl;
					Token top = allTokens.top();
					usedTokens.push(top);
                    Tree<string> rightParenTree(top.literal);
                    usedTrees.push(rightParenTree);
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
					Tree<string> temp(top.literal);
					usedTrees.push(temp);
					usedTokens.push(top);
					allTokens.pop();
					if (innerLoop == EXPNBTOKEN[outerLoop].size()-1) {//end of language reached
						while (usedTrees.size() > 0) {
							parentTree.addChild(usedTrees.front());
							usedTrees.pop();
						}
						return;
					}
				} else {
					if (outerLoop == EXPNBTOKEN.size()-1) {
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
