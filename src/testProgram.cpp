#include <string>
#include<iostream>
#include <stack>
#include <queue>
#include <vector>
#include "tree.h"
#ifdef _WIN32
#include <fcntl.h>
#include <io.h>
#endif

void setupWindowsWideChar() {
    #ifdef _WIN32
    _setmode(_fileno(stdout), _O_WTEXT);
    #endif
}
using namespace std;

class TokenOp{
public:
	enum TokenPiece {BEG,WORD1, WORD2,WORD3,LETTER1,LETTER2,LETTER3,DIGIT1,DIGIT2,DIGIT3,WORDCONVERT, LETTERCONVERT, DIGITCONVERTER};
	string value;
	TokenPiece tp ;
	TokenOp(string val, TokenPiece t){
		value = val;
		tp = t;
	}
};

class Token {
public:
	stack<TokenOp> pieceList;
	Token(){
		//pieceList.push(TokenOp("Letter2", TokenOp::TokenPiece::DIGIT2));
		//pieceList.push(TokenOp("Letter2", TokenOp::TokenPiece::DIGITCONVERTER));
		/*pieceList.push(TokenOp("Word2",TokenOp::TokenPiece::WORD2));
		pieceList.push(TokenOp("Word2",TokenOp::TokenPiece::WORD2));
		pieceList.push(TokenOp("DIGIT2", TokenOp::TokenPiece::DIGIT2));
		pieceList.push(TokenOp("DIGITCONVERTER", TokenOp::TokenPiece::DIGITCONVERTER));
		pieceList.push(TokenOp("digit1", TokenOp::TokenPiece::DIGIT1));
		pieceList.push(TokenOp("digit3", TokenOp::TokenPiece::DIGIT3));
		pieceList.push(TokenOp("digit1", TokenOp::TokenPiece::DIGIT1));
		pieceList.push(TokenOp("LETTERCONVERT", TokenOp::TokenPiece::LETTERCONVERT));
		
		pieceList.push(TokenOp("Letter1", TokenOp::TokenPiece::LETTER1));
		pieceList.push(TokenOp("Letter2", TokenOp::TokenPiece::LETTER2));
		pieceList.push(TokenOp("Letter3", TokenOp::TokenPiece::LETTER3));
		pieceList.push(TokenOp("wordconvert", TokenOp::TokenPiece::WORDCONVERT));
		pieceList.push(TokenOp("Word3",TokenOp::TokenPiece::WORD2));
		pieceList.push(TokenOp("Word1",TokenOp::TokenPiece::WORD1));
		pieceList.push(TokenOp("Beg1",TokenOp::TokenPiece::BEG));
		//pieceList.push(TokenOp("Word2",TokenOp::TokenPiece::WORD3));
		//pieceList.push_back(TokenOp("Word3",TokenOp::TokenPiece::WORD3));*/
		pieceList.push(TokenOp("Word3",TokenOp::TokenPiece::WORD3));
		pieceList.push(TokenOp("Word3",TokenOp::TokenPiece::WORD2));
		pieceList.push(TokenOp("Word1",TokenOp::TokenPiece::WORD1));
		pieceList.push(TokenOp("Beg1",TokenOp::TokenPiece::BEG));
	}

	stack<TokenOp> getPiece() const{
		return pieceList;
	}

};

//TokenOp::TokenPiece::WORD1,TokenOp::TokenPiece::WORD2, TokenOp::TokenPiece::WORDCONVERT , TokenOp::TokenPiece::DIGITCONVERTER
const vector<vector<TokenOp::TokenPiece> > language1 = {{TokenOp::TokenPiece::WORD1,TokenOp::TokenPiece::WORD2,TokenOp::TokenPiece::WORDCONVERT,TokenOp::TokenPiece::WORD2,TokenOp::TokenPiece::WORD2},{TokenOp::TokenPiece::WORD1,TokenOp::TokenPiece::WORD2,TokenOp::TokenPiece::WORD3},{TokenOp::TokenPiece::WORD3,TokenOp::TokenPiece::WORD2,TokenOp::TokenPiece::WORD1}};
const vector<vector<TokenOp::TokenPiece> > language2 = {{TokenOp::TokenPiece::LETTER1,TokenOp::TokenPiece::LETTER2},{TokenOp::TokenPiece::LETTER3,TokenOp::TokenPiece::LETTER2,TokenOp::TokenPiece::LETTER1,TokenOp::TokenPiece::LETTERCONVERT},{TokenOp::TokenPiece::LETTER1}};
const vector<vector<TokenOp::TokenPiece> > language3 = {{TokenOp::TokenPiece::DIGIT1,TokenOp::TokenPiece::DIGIT3,TokenOp::TokenPiece::DIGIT1,TokenOp::TokenPiece::DIGITCONVERTER},{TokenOp::TokenPiece::DIGIT3,TokenOp::TokenPiece::DIGIT1},{TokenOp::TokenPiece::DIGIT2},{TokenOp::TokenPiece::DIGIT2,TokenOp::TokenPiece::DIGIT1}};
//const Token alltokens[][3] = {{WORD1,WORD2}, {WORD3, WORD2}};

void language3Test(stack<TokenOp> &pieceList , Tree<string> &parentTree){
	for (int outerLoop = 0;  outerLoop < language3.size();outerLoop++){
		bool allPassed = true;
		for (int innerLoop =0; innerLoop < language3[outerLoop].size();innerLoop++){
			cout << outerLoop << " " << innerLoop << " " << pieceList.size() << endl;
			if ( pieceList.size() != 0 && language3[outerLoop][innerLoop] == TokenOp::TokenPiece::DIGITCONVERTER  ){
				//pieceList.pop();
				//language3Test(pieceList);
				std::wcout << "Call to test 3" << endl;
				//pieceList.pop();
				//usedTokens.push(pieceList.top());
				Tree<string> digitConvertTree(pieceList.top().value);
				pieceList.pop();
				language3Test(pieceList, digitConvertTree);
				parentTree.addChild(digitConvertTree);
				//retTree.addChild(tempTree);

				std::wcout<< "Out of language 3" << endl;
				continue;
			}else if ( pieceList.top().tp == language3[outerLoop][innerLoop]){
				TokenOp temp = pieceList.top();
				Tree<string> temp2(temp.value);
				parentTree.addChild(temp2);
				cout << "Got three in" <<endl;
				cout << "Stack size in 3 " << pieceList.size() << endl;
			}else{
				//cout << "Loop2 " << language1[outerLoop][innerLoop] << " " << pieceList.top().tp << endl;
				cout << "Loop 3 no match" << endl;
				cout << outerLoop << " " << innerLoop << endl;
				allPassed = false;
				break;
				//throw string("Broke in language 1");
			}
			if (pieceList.size() != 0){
				pieceList.pop();
			}
		}
		if (pieceList.size() ==0){
			return;
		}
		if (pieceList.size() == 0){
			throw string("Got all the way to language 3 and worked");
			//cout << "Got all the way to language 3 and worked" <<endl;
			break;
		}
		if (allPassed){
			return;
		}
	}
}

void language2Test(stack<TokenOp> &pieceList, Tree<string> &parentTree){
	//TokenOp temp;
	//Tree<string> retTree;
//	std::wcout << "Got in test 2" << endl;
	for (int outerLoop = 0;  outerLoop < language2.size();outerLoop++){
		bool allPassed = true;
		for (int innerLoop =0; innerLoop < language2[outerLoop].size();innerLoop++){
			//std::wcout << "print" << innerLoop << " " << pieceList.size() << endl;
			//cout << outerLoop << " " << innerLoop << " " << pieceList.size() << endl;
			if ( pieceList.size() != 0 && language2[outerLoop][innerLoop] == TokenOp::TokenPiece::LETTERCONVERT  ){
				std::wcout << "Call to test 3" << endl;
				//pieceList.pop();
				//usedTokens.push(pieceList.top());
				Tree<string> letterConvertTree(pieceList.top().value);
				pieceList.pop();
				language3Test(pieceList, letterConvertTree);
				parentTree.addChild(letterConvertTree);
				//retTree.addChild(tempTree);

				std::wcout<< "Out of language 3" << endl;
				continue;
				//language3Test(pieceList);
			}else if (pieceList.size() != 0 && pieceList.top().tp == language2[outerLoop][innerLoop]){
	//			std::wcout << "print got in to this" << endl;
				TokenOp temp = pieceList.top();
				Tree<string> temp2(temp.value);
				parentTree.addChild(temp2);
	//			std::wcout << "Got two in" <<endl;
	//			std::wcout << "Stack size in 2 " << pieceList.size() << endl;
			}else{
				//cout << "Loop2 " << language1[outerLoop][innerLoop] << " " << pieceList.top().tp << endl;
				cout << "Loop 2 no match" << endl;
				cout << outerLoop << " " << innerLoop << endl;
				allPassed = false;
				break;
				//throw string("Broke in language 1");
			}
			if (pieceList.size() != 0){
				pieceList.pop();
			}
			if (pieceList.size() ==0){
				return;
			}
			
		}
		if (allPassed){
			return;
		}

	}
	//return retTree;
}

void language1Test(stack<TokenOp> &pieceList, Tree<string> &parentTree){
	// TokenOp temp = pieceList.top();
	// Tree<string> retTree(temp.value);
	// pieceList.pop();
	//std::wcout << "print" <<language1[0].size() << endl;
	for (int outerLoop = 0;  outerLoop < language1.size();outerLoop++){
		stack<TokenOp> usedTokens;
		queue<Tree<string> > usedTrees ;
		for (int innerLoop =0; innerLoop < language1[outerLoop].size();innerLoop++){
			//cout << outerLoop << " " << innerLoop << " " << pieceList.size() << endl;
	//		std::wcout << "print" << innerLoop << endl;
			if ( pieceList.size() != 0 && language1[outerLoop][innerLoop] == TokenOp::TokenPiece::WORDCONVERT  ){
	//			std::wcout << "printP" << endl;
				usedTokens.push(pieceList.top());
				Tree<string> wordConvertTree(pieceList.top().value);
				pieceList.pop();
				language2Test(pieceList, wordConvertTree);
				usedTrees.push(wordConvertTree) ;
				//parentTree.addChild(wordConvertTree);
				//retTree.addChild(tempTree);

				std::wcout<< "Out of language 2" << endl;
				continue;
				
			}else if (pieceList.size() != 0 && pieceList.top().tp == language1[outerLoop][innerLoop]){
				TokenOp temp = pieceList.top();
				Tree<string> temp2(temp.value);
				usedTrees.push(temp2) ;
				//parentTree.addChild(temp2);
				//cout << "Got one in" <<endl;
	//			std::wcout << "Stack size" << endl;
			}else{
				//cout << pieceList.top().tp << endl;
			//	std::wcout << outerLoop << " " << innerLoop << " broke here" << endl;
				break;
				//throw string("Broke in language 1");
			}
			//cout << "Got to pop removing element" << pieceList.size() << endl;
			if (pieceList.size() != 0){
				usedTokens.push(pieceList.top());
				pieceList.pop();
			}
			//cout << "Got after removing element" << pieceList.size() << endl;
		}
		if (pieceList.size() == 0){
			while (usedTrees.size() >0){
				parentTree.addChild(usedTrees.front());
				usedTrees.pop();
			}
			return;
			cout << "completedTree" << endl;
			//return retTree;
		}else{
			//backtrack here
	//		std::wcout << "got to backtrack" << endl;
			while (usedTokens.size() != 0){
				cout << "Added element back " << usedTokens.top().tp << endl;
				pieceList.push(usedTokens.top());
				usedTokens.pop();
			}

		}

	}
	if (pieceList.size() != 0){
			throw string("Broke in language 1");
			//cout << "To next loop" << endl;
		}

	cout << "Got to end" << endl;
	//Tree<string> ret(temp.value);
	//return ret;
}



double doubleThrow(){
	throw string("Code broke");
	return 9;
}

string throwableCode() {
	doubleThrow();
	return "WORD1";
}

int main(int argc, char const *argv[])
{
	try {
		setupWindowsWideChar();
		Token tok;
		stack<TokenOp> toks = tok.getPiece();
		Tree<string> print(toks.top().value);
		// = language1Test(toks);
		toks.pop();
		/*Tree<string> temp(toks.top().value);
		toks.pop();
		Tree<string> temp2(toks.top().value);
		temp.addChild(temp2);
		print.addChild(temp);*/
		language1Test(toks,print);
		std::wcout << "returned the tree " << endl;
		std::wcout << print << endl;
		//language1Test(tok.getPiece());
	}catch (string error){
		std::wcout << "Error was throw" << endl;
		cout << error << endl;
	}
	return 0;
}
