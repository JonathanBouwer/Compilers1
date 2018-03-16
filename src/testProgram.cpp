#include <string>
#include<iostream>
#include <stack>
#include <queue>
#include <vector>
#include "tree.h"
using namespace std;

class TokenOp{
public:
	enum TokenPiece {WORD1, WORD2,WORD3,LETTER1,LETTER2,LETTER3,DIGIT1,DIGIT2,DIGIT3,WORDCONVERT, LETTERCONVERT, DIGITCONVERTER};
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
		pieceList.push(TokenOp("Letter2", TokenOp::TokenPiece::DIGIT2));
		//pieceList.push(TokenOp("Letter2", TokenOp::TokenPiece::DIGITCONVERTER));
		pieceList.push(TokenOp("Letter2", TokenOp::TokenPiece::DIGIT1));
		pieceList.push(TokenOp("Letter2", TokenOp::TokenPiece::DIGIT3));
		pieceList.push(TokenOp("Letter2", TokenOp::TokenPiece::DIGIT1));
		//pieceList.push(TokenOp("Letter2", TokenOp::TokenPiece::LETTERCONVERT));
		pieceList.push(TokenOp("Letter2", TokenOp::TokenPiece::LETTER1));
		pieceList.push(TokenOp("Letter2", TokenOp::TokenPiece::LETTER2));
		pieceList.push(TokenOp("Letter2", TokenOp::TokenPiece::LETTER3));
		//pieceList.push(TokenOp("Letter2", TokenOp::TokenPiece::WORDCONVERT));
		pieceList.push(TokenOp("Word3",TokenOp::TokenPiece::WORD2));
		pieceList.push(TokenOp("Word1",TokenOp::TokenPiece::WORD1));
		//pieceList.push(TokenOp("Word2",TokenOp::TokenPiece::WORD3));
		//pieceList.push_back(TokenOp("Word3",TokenOp::TokenPiece::WORD3));
		/*pieceList.push(TokenOp("Word3",TokenOp::TokenPiece::WORD3));
		pieceList.push(TokenOp("Word3",TokenOp::TokenPiece::WORD2));
		pieceList.push(TokenOp("Word1",TokenOp::TokenPiece::WORD1));*/
	}

	stack<TokenOp> getPiece() const{
		return pieceList;
	}

};

//TokenOp::TokenPiece::WORD1,TokenOp::TokenPiece::WORD2, TokenOp::TokenPiece::WORDCONVERT
const vector<vector<TokenOp::TokenPiece> > language1 = {{TokenOp::TokenPiece::WORD1,TokenOp::TokenPiece::WORD2, TokenOp::TokenPiece::WORDCONVERT},{TokenOp::TokenPiece::WORD1,TokenOp::TokenPiece::WORD2,TokenOp::TokenPiece::WORD3},{TokenOp::TokenPiece::WORD3,TokenOp::TokenPiece::WORD2,TokenOp::TokenPiece::WORD1}};
const vector<vector<TokenOp::TokenPiece> > language2 = {{TokenOp::TokenPiece::LETTER1,TokenOp::TokenPiece::LETTER2},{TokenOp::TokenPiece::LETTER3,TokenOp::TokenPiece::LETTER2,TokenOp::TokenPiece::LETTER1, TokenOp::TokenPiece::LETTERCONVERT},{TokenOp::TokenPiece::LETTER1}};
const vector<vector<TokenOp::TokenPiece> > language3 = {{TokenOp::TokenPiece::DIGIT1,TokenOp::TokenPiece::DIGIT3,TokenOp::TokenPiece::DIGIT1, TokenOp::TokenPiece::DIGITCONVERTER},{TokenOp::TokenPiece::DIGIT3,TokenOp::TokenPiece::DIGIT1},{TokenOp::TokenPiece::DIGIT2},{TokenOp::TokenPiece::DIGIT2,TokenOp::TokenPiece::DIGIT1}};
//const Token alltokens[][3] = {{WORD1,WORD2}, {WORD3, WORD2}};

void language3Test(stack<TokenOp> &pieceList){
	for (int outerLoop = 0;  outerLoop < language3.size();outerLoop++){
			for (int innerLoop =0; innerLoop < language3[outerLoop].size();innerLoop++){
				cout << outerLoop << " " << innerLoop << " " << pieceList.size() << endl;
				if ( pieceList.size() != 0 && language3[outerLoop][innerLoop] == TokenOp::TokenPiece::DIGITCONVERTER  ){
					//pieceList.pop();
					language3Test(pieceList);
				}else if ( pieceList.top().tp == language3[outerLoop][innerLoop]){

					cout << "Got three in" <<endl;
					cout << "Stack size in 3 " << pieceList.size() << endl;
				}else{
					//cout << "Loop2 " << language1[outerLoop][innerLoop] << " " << pieceList.top().tp << endl;
					cout << "Loop 3 no match" << endl;
					cout << outerLoop << " " << innerLoop << endl;
					break;
					//throw string("Broke in language 1");
				}
				pieceList.pop();
			}
			if (pieceList.size() == 0){
				throw string("Got all the way to language 3 and worked");
				//cout << "Got all the way to language 3 and worked" <<endl;
				break;
			}

		}
}

void language2Test(stack<TokenOp> &pieceList){
	for (int outerLoop = 0;  outerLoop < language2.size();outerLoop++){
		for (int innerLoop =0; innerLoop < language2[outerLoop].size();innerLoop++){
			//cout << outerLoop << " " << innerLoop << " " << pieceList.size() << endl;
			if ( pieceList.size() != 0 && language2[outerLoop][innerLoop] == TokenOp::TokenPiece::LETTERCONVERT  ){
				cout << "Call to test 3" << endl;
				//pieceList.pop();
				language3Test(pieceList);
			}else if ( pieceList.top().tp == language2[outerLoop][innerLoop]){

				cout << "Got two in" <<endl;
				cout << "Stack size in 2 " << pieceList.size() << endl;
			}else{
				//cout << "Loop2 " << language1[outerLoop][innerLoop] << " " << pieceList.top().tp << endl;
				cout << "Loop 2 no match" << endl;
				cout << outerLoop << " " << innerLoop << endl;
				break;
				//throw string("Broke in language 1");
			}
			pieceList.pop();
		}

	}
}

void language1Test(stack<TokenOp> &pieceList){
	for (int outerLoop = 0;  outerLoop < language1.size();outerLoop++){
		stack<TokenOp> usedTokens;
		for (int innerLoop =0; innerLoop < language1[outerLoop].size();innerLoop++){
			//cout << outerLoop << " " << innerLoop << " " << pieceList.size() << endl;
			if ( pieceList.size() != 0 && language1[outerLoop][innerLoop] == TokenOp::TokenPiece::WORDCONVERT  ){
				cout << "Called test 2" << endl;
				//usedTokens.push(pieceList.top());
				//pieceList.pop();
				language2Test(pieceList);
				break;
			}else if ( pieceList.top().tp == language1[outerLoop][innerLoop]){

				//cout << "Got one in" <<endl;
				cout << "Stack size" << pieceList.size() << endl;
			}else{
				//cout << pieceList.top().tp << endl;
				cout << outerLoop << " " << innerLoop << " broke here" << endl;
				break;
				//throw string("Broke in language 1");
			}
			//cout << "Got to pop removing element" << pieceList.size() << endl;
			usedTokens.push(pieceList.top());
			pieceList.pop();
			//cout << "Got after removing element" << pieceList.size() << endl;
		}
		if (pieceList.size() == 0){
			cout << "completedTree" << endl;
			return;
		}else{
			//backtrack here
			cout << "got to backtrack" << endl;
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
		
		Token tok;
		stack<TokenOp> toks = tok.getPiece();
		language1Test(toks);
		//language1Test(tok.getPiece());
	}catch (string error){
		cout << error << endl;
	}
	return 0;
}