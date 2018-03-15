#include <stack>
#include <deque>
#include <vector>
#include <string>
#include <regex>
#include <sstream>
#include <fstream>

#include "tokenType.h"
#include "tokenMatchers.h"

using namespace std;

string readFile(const string& fileName) {
    stringstream ss;
    ifstream input(fileName);
    ss << input.rdbuf();
    input.close();
    return ss.str();
}

int nextTokenLength(const string& input, int currentPos) {
    int tokenLength = 0;
    bool validMatchers = true;
    while (validMatchers) {
        ++tokenLength;
        validMatchers = false;
        auto tokenStart = input.begin() + currentPos;
        auto tokenEnd = tokenStart + tokenLength;
        for (TokenRegex& matcher : matchers) {
            if (regex_match(tokenStart, tokenEnd, matcher.matcher)) {
                validMatchers = true;
                break;
            }
        }
    }        
    --tokenLength;
    return tokenLength;
}

int findNextMatchingChar(const string& input, int currentPos, char c, int row, int column) {
    int index = 1;
    while (input[currentPos + index] != c) {
        ++index;
        if (currentPos + index >= input.length()) {
            throw "Lexing Error: " + string(1, c) + " character at line " + to_string(row) + ":" + to_string(column) + " was never closed\n";
        }
    }
    return index;
}

stack<Token> tokenize(const string& input) {
    vector<Token> tokenStream;
    int row = 0, column = 0, currentPos = 0;
    while (currentPos < input.length()) {
        int tokenLength = nextTokenLength(input, currentPos);
        // Parse token and add it to the tree
        auto tokenStart = input.begin() + currentPos;
        auto tokenEnd = tokenStart + tokenLength;
        for (TokenRegex& matcher : matchers) {
            if (regex_match(tokenStart, tokenEnd, matcher.matcher)) {
                // Newlines and White space not added to token stream
                if (matcher.type == NEWLINE) {
                    ++row;
                    column = 0;
                    break;
                } else if (matcher.type == WHITESPACE) {
                    column += tokenLength;
                    break;
                }
                
                // Literal Strings and Chars require start and end quotes
                if (matcher.type == LITERAL_STRING) {
					tokenLength += findNextMatchingChar(input, currentPos, '"', row, column);
				} else if (matcher.type == LITERAL_CHAR) {
					tokenLength += findNextMatchingChar(input, currentPos, '\'', row, column);
				}
                
                // Add Token to stream
                Token token;
                token.type = matcher.type;
				token.literal = string(input, currentPos, tokenLength);
                token.row = row;
                token.column = column;
				tokenStream.push_back(token);
                
                // Adjust column
                column += tokenLength;
                break;
            }
        }
        
        currentPos += tokenLength;
    }
    
    Token endOfFile;
    endOfFile.type = END_OF_FILE;
    endOfFile.literal = "";
    endOfFile.row = row;
    endOfFile.column = column;
    tokenStream.push_back(endOfFile);
    
    return stack<Token>(deque<Token>(tokenStream.begin(), tokenStream.end()));
}

stack<Token> tokenizeFile(const string& fileName) {
    string input = readFile(fileName);
    return tokenize(input);
}

