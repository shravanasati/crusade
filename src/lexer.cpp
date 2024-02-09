#include "tokens.hpp"
#include <bits/stdc++.h>
#include <iostream>

class Lexer {
private:
    const std::string& infix;
    int position;
    char currentChar;

    void advance() {
        position++;
        currentChar = infix.at(position);
    }

    bool isDigit(char c) { return c >= '0' && c <= '9'; }

    bool isDecimal(char c) { return c == '.'; }

    Token makeNumberToken() {
        std::string number;
        while (true) {
            if (isDigit(currentChar) || isDecimal(currentChar)) {
                number += currentChar;
                advance();
            } else {
                throw std::logic_error("invalid character");
            }
        }
        return NumberToken(number);
    }

    void reportError(std::string error) {
        std::cout << infix << "\n";
        std::cout << std::string(position - 1, ' ');
        std::cout << "^ " << error << std::endl;
    }

public:
    Lexer(const std::string& input) : infix(input) { position = -1; }

    std::vector<Token> tokenize() {
        std::vector<Token> tokens;
        advance();
        bool reachedEnd = false;
        while (!reachedEnd) {
            if (std::isspace(currentChar)) {
                goto next;
            }

            if (currentChar == '+') {
                tokens.push_back(PlusToken());
            } else if (currentChar == '-') {
                tokens.push_back(MinusToken());
            } else if (currentChar == '*') {
                tokens.push_back(MultiplyToken());
            } else if (currentChar == '/') {
                if (position == (infix.length() - 1)) {
                    // no need to check for floor division if the end of input
                    // is reached
                    tokens.push_back(DivideToken());
                } else {
                    // try advancing to check for floor division operator
                    advance();
                    if (currentChar == '/') {
                        tokens.push_back(FloorDivideToken());
                    } else {
                        tokens.push_back(DivideToken());
                        // go back one character since we advanced once more to
                        // check next character
                        position--;
                    }
                }
            } else if (currentChar == '^') {
                tokens.push_back(ExponentToken());
            } else if (currentChar == '(') {
                tokens.push_back(LPARENToken());
            } else if (currentChar == ')') {
                tokens.push_back(RPARENToken());
            } else {
                try {
                    auto numberToken = makeNumberToken();
                } catch (...) {
                    reportError("invalid character");
                    std::vector<Token> emptyvector;
                    return emptyvector;
                }
            }

        next: {
            try {
                advance();
            } catch (std::out_of_range) {
                reachedEnd = true;
            }
        }
        }
        return tokens;
    }
};
