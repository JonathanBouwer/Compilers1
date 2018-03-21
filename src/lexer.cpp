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

/*
** Simple method to read a file into a string
*/
string readFile(const string& fileName) {
    stringstream ss;
    ifstream input(fileName);
    if(!input.is_open()) {
        throw string("File not found: ") + fileName;
    }    
    ss << input.rdbuf();
    input.close();
    return ss.str();
}

/*
** Wrapper method to throw unidentified token exception
*/
void unidentifiedToken(const string& input, int row, int column) {
    stringstream ss;
    ss << "Lexing Error: Unidentified token at " << row << ":" << column+1;
    ss << " - \"" << string(input, column) << "\"\n";
    throw ss.str();
}

/*
** Method to find the longest token starting at currentPos
** linear searches on the length until no tokens match and
** then returns length - 1, or throws an error if no token
** is found
*/
int nextTokenLength(const string& input, int column, int row) {
    int tokenLength = 0;
    bool validMatchers = true;
    while (validMatchers) {
        ++tokenLength;
        validMatchers = false;
        auto tokenStart = input.begin() + column;
        auto tokenEnd = tokenStart + tokenLength;
        for (TokenRegex& matcher : matchers) {
            if (regex_match(tokenStart, tokenEnd, matcher.matcher)) {
                validMatchers = true;
                break;
            }
        }
    }
    --tokenLength;

    // If no token is found throw an error
    if (tokenLength == 0) {
        unidentifiedToken(input, row, column);
    }
    return tokenLength;
}

/*
** Method to find the position of a character in a string or throws an
** error if not found
*/
int findNextMatchingChar(const string& input, int column, char c, int row) {
    int pos = input.find(c, column + 1);
    if (pos < 0) {
        stringstream ss;
        ss << "Lexing Error: " << c << " character at ";
        ss << row << ":" << column << " was never closed\n";
        throw ss.str();
    }
    return pos - column;
}

/*
** Simple method to ensure next character matches a given character and is not
** out of bounds. Throws an error if these conditions are not met
*/
void nextCharMatches(const string& input, int column, char c, int row) {
    if (input.length() <= column + 1 || input[column + 1] != c) {
        unidentifiedToken(input, row, column);
    }
}

/*
** Method to take an input string and convert it into a stream of tokens.
** If an error occurs during lexing then a string is thrown.
*/
stack<Token> tokenize(const string& input) {
    vector<Token> tokenStream;
    int row = 0;
    stringstream ss(input);
    string currentLine;
    while (getline(ss, currentLine)) {
        ++row;
        int column = 0;
        // Loop through current line
        while (column < currentLine.length()) {
            int tokenLength = nextTokenLength(currentLine, column, row);

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
                    /* The following 3 operators are 2 characters long and do not match
                    ** the identifier regex during longest match so they instead match
                    ** a single char and then checking that the next char matches.
                    */
                    else if (matcher.type == OPERATOR_AND) {
                        nextCharMatches(currentLine, column, '&', row);
                        tokenLength += 1;
                    } else if (matcher.type == OPERATOR_OR) {
                        nextCharMatches(currentLine, column, '|', row);
                        tokenLength += 1;
                    } else if (matcher.type == OPERATOR_NEQ) {
                        nextCharMatches(currentLine, column, '=', row);
                        tokenLength += 1;
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

    return stack<Token>(deque<Token>(tokenStream.rbegin(), tokenStream.rend()));
}

/*
** Wrapper Method to read file and tokenize it.
*/
stack<Token> tokenizeFile(const string& fileName) {
    string input = readFile(fileName);
    return tokenize(input);
}
