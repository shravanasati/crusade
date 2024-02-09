#ifndef LEXER_HPP
#define LEXER_HPP

#include "tokens.hpp"
#include <vector>
#include <string>

class Lexer {
private:
    const std::string& infix;
    int position;
    char currentChar;

    void advance();
    bool isDigit(char c);
    bool isDecimal(char c);
    Token makeNumberToken();
    void reportError(std::string error);

public:
    Lexer(const std::string& input);
    std::vector<Token> tokenize();
};

#endif // LEXER_HPP
