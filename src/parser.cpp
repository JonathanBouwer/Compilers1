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

//
	Tree<Token> funcions(stack<Token> &allTokens){

	}

	Tree<Token> statements(stack<Token> &allTokens){

	}

	Tree<Token> expressions(stack<Token> &allTokens){

	}
}
