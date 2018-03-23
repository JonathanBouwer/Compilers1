#pragma once
#include <ostream>
#include <string>
#include <regex>

/*
** List of possible tokens in our language
*/
enum TokenType {
    IDENTIFIER_FUNCTION,
    IDENTIFIER_VARIABLE,
    LITERAL_CHAR,
    LITERAL_INTEGER,
    LITERAL_STRING,
    KEYWORD_BE,
    KEYWORD_DO,
    KEYWORD_EXECUTE,
    KEYWORD_FORCE,
    KEYWORD_IF,
    KEYWORD_LET,
    KEYWORD_PRETEXT,
    KEYWORD_THEN,
    KEYWORD_THE_END,
    KEYWORD_TRANSMIT,
    KEYWORD_WHILE,
    KEYWORD_YOU_MUST,
    SEPARATOR_COMMA,
    SEPARATOR_LEFT_PAREN,
    SEPARATOR_RIGHT_PAREN,
    SEPARATOR_SEMICOLON,
    OPERATOR_AND,
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
    FUNCTIONS,
    EXPRESSIONNB,
    EXPRESSIONB,
    PARAMS,
    EMPTY
};

/*
** Token data structure containing TokenType type, string literal and int row & column
*/
struct Token {
    TokenType type;
    std::string literal;
    int row, column;
    
    friend std::ostream& operator<<(std::ostream& out, const Token& token) {
        out << "(";
        switch (token.type) {
            case IDENTIFIER_FUNCTION : {
                out << "IDENTIFIER_FUNCTION<" << token.literal << ">";
                break;
            }
            case IDENTIFIER_VARIABLE : {
                out << "IDENTIFIER_VARIABLE<" << token.literal << ">";
                break;
            }
            case LITERAL_CHAR : {
                out << "LITERAL_CHAR<" << token.literal << ">";
                break;
            }
            case LITERAL_INTEGER : {
                out << "LITERAL_INTEGER<" << token.literal << ">";
                break;
            }
            case LITERAL_STRING : {
                out << "LITERAL_STRING<" << token.literal << ">";
                break;
            }
            case KEYWORD_BE : {
                out << "KEYWORD_BE";
                break;
            }
            case KEYWORD_DO : {
                out << "KEYWORD_DO";
                break;
            }
            case KEYWORD_EXECUTE : {
                out << "KEYWORD_EXECUTE";
                break;
            }
            case KEYWORD_FORCE : {
                out << "KEYWORD_FORCE";
                break;
            }
            case KEYWORD_IF : {
                out << "KEYWORD_IF";
                break;
            }
            case KEYWORD_LET : {
                out << "KEYWORD_LET";
                break;
            }
            case KEYWORD_PRETEXT : {
                out << "KEYWORD_PRETEXT";
                break;
            }
            case KEYWORD_THEN : {
                out << "KEYWORD_THEN";
                break;
            }
            case KEYWORD_THE_END : {
                out << "KEYWORD_THE_END";
                break;
            }
            case KEYWORD_TRANSMIT : {
                out << "KEYWORD_TRANSMIT";
                break;
            }
            case KEYWORD_WHILE : {
                out << "KEYWORD_WHILE";
                break;
            }
            case KEYWORD_YOU_MUST : {
                out << "KEYWORD_YOU_MUST";
                break;
            }
            case SEPARATOR_COMMA : {
                out << "SEPARATOR_COMMA";
                break;
            }
            case SEPARATOR_LEFT_PAREN : {
                out << "SEPARATOR_LEFT_PAREN";
                break;
            }
            case SEPARATOR_RIGHT_PAREN : {
                out << "SEPARATOR_RIGHT_PAREN";
                break;
            }
            case SEPARATOR_SEMICOLON : {
                out << "SEPARATOR_SEMICOLON";
                break;
            }
            case OPERATOR_AND : {
                out << "OPERATOR_AND";
                break;
            }
            case OPERATOR_DIVIDE : {
                out << "OPERATOR_DIVIDE";
                break;
            }
            case OPERATOR_GT : {
                out << "OPERATOR_GT";
                break;
            }
            case OPERATOR_GTE : {
                out << "OPERATOR_GTE";
                break;
            }
            case OPERATOR_IS : {
                out << "OPERATOR_IS";
                break;
            }
            case OPERATOR_LT : {
                out << "OPERATOR_LT";
                break;
            }
            case OPERATOR_LTE : {
                out << "OPERATOR_LTE";
                break;
            }
            case OPERATOR_MINUS : {
                out << "OPERATOR_MINUS";
                break;
            }
            case OPERATOR_MOD : {
                out << "OPERATOR_MOD";
                break;
            }
            case OPERATOR_NEQ : {
                out << "OPERATOR_NEQ";
                break;
            }
            case OPERATOR_OR : {
                out << "OPERATOR_OR";
                break;
            }
            case OPERATOR_PLUS : {
                out << "OPERATOR_PLUS";
                break;
            }
            case OPERATOR_POW : {
                out << "OPERATOR_POW";
                break;
            }
            case OPERATOR_TIMES : {
                out << "OPERATOR_TIMES";
                break;
            }
        }
        out << ", Line:" << token.row;
        out << ", Character:" << token.column;
        out << ")";
        return out;
    }
};

/*
** Token regex structure intended for keeping a map of TokenTypes to regular expressions
*/
struct TokenRegex {
    TokenType type;
    std::regex matcher;
    
    TokenRegex(const TokenType Type, const std::regex& Matcher) {
        type = Type;
        matcher = Matcher;
    }
};