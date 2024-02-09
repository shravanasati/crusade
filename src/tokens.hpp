#ifndef TOKENS_HPP
#define TOKENS_HPP

#include <string>

enum class Associativity { LEFT, RIGHT };

enum class TokenType {
    NUMBER,
    PLUS_OP,
    MINUS_OP,
    MULTIPLY_OP,
    DIVIDE_OP,
    FLOOR_DIVIDE_OP,
    EXPONENT_OP,
    LPAREN,
    RPAREN
};

class Token {
protected:
    std::string value;
    TokenType tokenType;
    int precedence;
    Associativity associativity;

public:
    Token(const std::string& value, TokenType tokenType, int precedence,
          Associativity associativity);

    std::string getValue() const;
    TokenType getTokenType() const;
    int getPrecedence() const;
    Associativity getAssociativity() const;

    bool operator==(const Token& other) const;
};

Token NumberToken(std::string value);
Token PlusToken();
Token MinusToken();
Token MultiplyToken();
Token DivideToken();
Token FloorDivideToken();
Token ExponentToken();
Token LPARENToken();
Token RPARENToken();

#endif // TOKENS_HPP
