#include <string>
#include<iostream>
#include "tree.h"
#include "tokenType.h"
#include <stack>
#include <vector>
using namespace std;

class Parser{
	Parser(){}

	Tree lib(stack<Token> &allTokens){
		Token topItem = allTokens.top();
		if (topItem.type == tokenType.KEYWORD_PRETEXT) {
			Tree parseTree<Token>(topItem);
			allTokens.pop();
			parseTree.addChild(functions(allTokens);
		} else {
			printf("Failure on character %s at row %d, column %d. Expected the Pretext keyword, I did. This is why you fail. \n", topItem.literal, topItem.row, topItem.column);
			exit(0);
		}
	}

	&vector<Tree> funcions(stack<Token> &allTokens){

	}

	&vector<Tree> statements(stack<Token> &allTokens){

	}

	&vector<Tree> expressions(stack<Token> &allTokens){

	}
}
