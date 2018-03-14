#include <string>
#include "tree.h"
#include "tokenType.h"
#include <stack>
using namespace std

Tree lib(stack<Token> &allTokens){
	tokenType topItem = allTokens.top();
	if (topItem.type == tokenType.KEYWORD_PRETEXT)
	Tree parseTree<Token>(topItem);
	allTokens.pop();

}

&Tree funcions(stack<Token> &allTokens){

}

&Tree statements(stack<Token> &allTokens){

}

&Tree expressions(stack<Token> &allTokens){

}