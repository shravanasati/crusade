#include <string>

enum class Associativity { LEFT, RIGHT };

enum class TokenType {NUMBER, PLUS_OP, MINUS_OP, MULTIPLY_OP, DIVIDE_OP, FLOOR_DIVIDE_OP, EXPONENT_OP, LPAREN, RPAREN};

class Token {
protected:
    std::string value;
    TokenType tokenType;
    int precedence;
    Associativity associativity;

public:
    Token(const std::string& value, TokenType tokenType, int precedence, Associativity associativity)
        : value(value), tokenType(tokenType), precedence(precedence), associativity(associativity) {}


    std::string getValue() const { return value; }
    TokenType getTokenType() const { return tokenType; }
    int getPrecedence() const { return precedence; }
    Associativity getAssociativity() const { return associativity; }

    bool operator == (const Token& other) const {
        return tokenType == other.tokenType;
    }
};

Token NumberToken(std::string value) {
    return Token(value, TokenType::NUMBER, 0, Associativity::LEFT);
}

Token PlusToken() {
    return Token("+", TokenType::PLUS_OP, 1, Associativity::LEFT);
}

Token MinusToken() {
    return Token("-", TokenType::MINUS_OP,1, Associativity::LEFT);
}

Token MultiplyToken() {
    return Token("*", TokenType::MULTIPLY_OP,2, Associativity::LEFT);
}


Token DivideToken() {
    return Token("/", TokenType::DIVIDE_OP,2, Associativity::LEFT);
}

Token FloorDivideToken() {
    return Token("//", TokenType::FLOOR_DIVIDE_OP,2, Associativity::LEFT);
}

Token ExponentToken() {
    return Token("^", TokenType::EXPONENT_OP, 3, Associativity::RIGHT);
}

Token LPARENToken() {
    return Token("(", TokenType::LPAREN, 0, Associativity::LEFT);
}

Token RPARENToken() {
    return Token(")", TokenType::RPAREN, 0, Associativity::LEFT);
}