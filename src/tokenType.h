#pragma once
#include <string>
#include <regex>

enum TokenType {
    IDENTIFIER_FUNCTION,
    IDENTIFIER_VARIABLE,
    LITERAL_CHAR,
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
    KEYWORD_YOU_MUST,
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
    END_OF_FILE,
    WHITESPACE,
    NEWLINE
};

struct Token {
    TokenType type;
    std::string literal;
    int row, column;
};

struct TokenRegex {
    TokenType type;
    std::regex matcher;
    
    TokenRegex(const TokenType Type, const std::regex& Matcher) {
        type = Type;
        matcher = Matcher;
    }
};