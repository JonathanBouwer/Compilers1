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

int findNextMatchingChar(const string& input, int column, char c, int row) {
    int index = 1;
    while (input[column + index] != c) {
        ++index;
        if (column + index >= input.length()) {
            stringstream ss;
            ss << "Lexing Error: " << c << " character at ";
            ss << row << ":" << column << " was never closed\n";
            throw ss.str();
        }
    }
    return index;
}

stack<Token> tokenize(const string& input) {
    vector<Token> tokenStream;
    int row = 0;
    stringstream ss(input);
    string currentLine;
    while (getline(ss, currentLine)) {
        ++row;
        int column = 0;
        while (column < currentLine.length()) {
            int tokenLength = nextTokenLength(currentLine, column);
            
            if (tokenLength == 0) {
                stringstream ss;
                ss << "Lexing Error: Unidentified token at " << row << ":" << column+1;
                ss << " - \"" << string(currentLine, column) << "\"\n";
                throw ss.str();
            }
            
            // Parse token and add it to the tree
            auto tokenStart = currentLine.begin() + column;
            auto tokenEnd = tokenStart + tokenLength;
            for (TokenRegex& matcher : matchers) {
                if (regex_match(tokenStart, tokenEnd, matcher.matcher)) {
                    // White space is not added to token stream
                    if (matcher.type == WHITESPACE) {
                        break;
                    }
                    
                    // Literal Strings and Chars require end quotes
                    if (matcher.type == LITERAL_STRING) {
                        tokenLength += findNextMatchingChar(currentLine, column, '"', row);
                    } else if (matcher.type == LITERAL_CHAR) {
                        tokenLength += findNextMatchingChar(currentLine, column, '\'', row);
                    }
                    
                    // Add Token to stream
                    Token token;
                    token.type = matcher.type;
                    token.literal = string(currentLine, column, tokenLength);
                    token.row = row;
                    token.column = column+1;
                    tokenStream.push_back(token);
                    break;
                }
            }
            column += tokenLength;
        }
    }
    
    
    Token endOfFile;
    endOfFile.type = END_OF_FILE;
    endOfFile.literal = "";
    endOfFile.row = row+1;
    endOfFile.column = 1;
    tokenStream.push_back(endOfFile);
    
    return stack<Token>(deque<Token>(tokenStream.rbegin(), tokenStream.rend()));
}

stack<Token> tokenizeFile(const string& fileName) {
    string input = readFile(fileName);
    return tokenize(input);
}
