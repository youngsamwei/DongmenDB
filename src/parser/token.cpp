//
// Created by maokw on 2020-03-17.
//

#include <ostream>
#include <parser/Token.h>

std::ostream &operator<<(std::ostream &os, TokenType type) {
    switch (type) {
        case TOKEN_NOT:
            os << "NOT";
            break;
        case TOKEN_AND:
            os << "AND";
            break;
        case TOKEN_OR:
            os << "OR";
            break;
        case TOKEN_PLUS:
            os << '+';
            break;
        case TOKEN_MINUS:
            os << '-';
            break;
        case TOKEN_MULTIPLY:
            os << '*';
            break;
        case TOKEN_DIVIDE:
            os << '/';
            break;
        case TOKEN_GT:
            os << '>';
            break;
        case TOKEN_LT:
            os << '<';
            break;
        case TOKEN_LE:
            os << '<=';
            break;
        case TOKEN_GE:
            os << '>=';
            break;
        case TOKEN_EQ:
            os << '=';
            break;
        case TOKEN_NOT_EQUAL:
            os << '!=';
            break;
        case TOKEN_COMMA:
            os << ',';
            break;
        case TOKEN_ASSIGNMENT:
            os << '=';
            break;
        case TOKEN_LIKE:
            os << "LIKE";
            break;
        case TOKEN_IN:
            os << "IN";
            break;
        default:
            os << "<token-unknown>";
            break;
    }

    return os;

}