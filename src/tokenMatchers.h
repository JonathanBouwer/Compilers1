#pragma once
#include <vector>
#include "tokenType.h"

std::vector<TokenRegex> matchers = {
	{WHITESPACE, std::regex("(\\s)*")},
	{SEPARATOR_LEFT_PAREN, std::regex("\\(")},
	{SEPARATOR_RIGHT_PAREN, std::regex("\\)")},
	{SEPARATOR_SEMICOLON, std::regex(";")},
	{KEYWORD_IF, std::regex("if")},
	{KEYWORD_DO, std::regex("do")},
	{KEYWORD_EXECUTE, std::regex("execute")},
	{KEYWORD_FORCE, std::regex("force")},
	{KEYWORD_LET, std::regex("let")},
	{KEYWORD_PRETEXT, std::regex("a_long_time_ago_in_a_galaxy_far_far_away")},
	{KEYWORD_THEN, std::regex("then")},
	{KEYWORD_THE_END, std::regex("the_end")},
	{KEYWORD_TRANSMIT, std::regex("transmit")},
	{KEYWORD_WHILE, std::regex("while")},
	{KEYWORD_YOU_MUST, std::regex("you_must")},
	{OPERATOR_AND, std::regex("&")},
	{OPERATOR_BE, std::regex("be")},
	{OPERATOR_COMMA, std::regex(",")},
	{OPERATOR_DIVIDE, std::regex("/")},
	{OPERATOR_GT, std::regex(">")},
	{OPERATOR_GTE, std::regex(">=")},
	{OPERATOR_IS, std::regex("is")},
	{OPERATOR_LT, std::regex("<")},
	{OPERATOR_LTE, std::regex("<=")},
	{OPERATOR_MINUS, std::regex("-")},
	{OPERATOR_MOD, std::regex("%")},
	{OPERATOR_NEQ, std::regex("!")},
	{OPERATOR_OR, std::regex("\\|")},
	{OPERATOR_PLUS, std::regex("\\+")},
	{OPERATOR_POW, std::regex("\\^")},
	{OPERATOR_TIMES, std::regex("\\*")},
	{LITERAL_INTEGER, std::regex("([0-9])*")},
	{LITERAL_CHAR, std::regex("'")},
	{LITERAL_STRING, std::regex("\"")},
	{IDENTIFIER_FUNCTION, std::regex("order_([a-zA-Z0-9_])*")},
	{IDENTIFIER_VARIABLE, std::regex("[a-z]([a-zA-Z0-9_])*")}
};