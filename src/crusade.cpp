#include <bits/stdc++.h>
#include <iostream>

/* * STRINGUTILS * */
// trim from start (in place)
inline void ltrim(std::string& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
                return !std::isspace(ch);
            }));
}

// trim from end (in place)
inline void rtrim(std::string& s) {
    s.erase(std::find_if(s.rbegin(), s.rend(),
                         [](unsigned char ch) { return !std::isspace(ch); })
                .base(),
            s.end());
}

// trim string from both ends
inline void trim(std::string& s) {
    ltrim(s);
    rtrim(s);
}

/* * TOKENS * */
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
          Associativity associativity)
        : value(value), tokenType(tokenType), precedence(precedence),
          associativity(associativity) {}

    std::string getValue() const { return value; }
    TokenType getTokenType() const { return tokenType; }
    int getPrecedence() const { return precedence; }
    Associativity getAssociativity() const { return associativity; }

    bool operator==(const Token& other) const {
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
    return Token("-", TokenType::MINUS_OP, 1, Associativity::LEFT);
}

Token MultiplyToken() {
    return Token("*", TokenType::MULTIPLY_OP, 2, Associativity::LEFT);
}

Token DivideToken() {
    return Token("/", TokenType::DIVIDE_OP, 2, Associativity::LEFT);
}

Token FloorDivideToken() {
    return Token("//", TokenType::FLOOR_DIVIDE_OP, 2, Associativity::LEFT);
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

/* * LEXER * */

struct makeTokenResult {
    Token token;
    bool success;
};

class Lexer {
private:
    const std::string& infix;
    int position;
    char currentChar;
    int rank;
    std::stack<int> bracketPositions;

    void advance() {
        position++;
        currentChar = infix.at(position);
    }

    bool isDigit(char c) { return c >= '0' && c <= '9'; }

    bool isDecimal(char c) { return c == '.'; }

    makeTokenResult makeNumberToken() {
        std::string number;
        int decimalCount = 0;
        while (true) {
            if (isDigit(currentChar) || isDecimal(currentChar)) {
                number += currentChar;
                if (isDecimal(currentChar)) {
                    decimalCount++;
                }
                if (decimalCount > 1) {
                    reportError("multiple decimals in a number");
                    return makeTokenResult{NumberToken("0"), false};
                }
                try {
                    advance();
                } catch (std::out_of_range) {
                    return makeTokenResult{NumberToken(number), true};
                }
            } else {
                // since current char is not a digit or a decimal
                // it maybe a operator which will be advanced upon
                // inside the loop, so decrease the position by 1
                position--;
                break;
            }
        }
        // convert .52 => 0.52
        if (number[0] == '.') {
            number = "0" + number;
        }
        // convert 52. => 52.0
        if (number.at(number.size() - 1) == '.') {
            number += "0";
        }
        // convert 52 => 52.0
        if (decimalCount == 0) {
            number += ".0";
        }
        return makeTokenResult{NumberToken(number), true};
    }

    void reportError(std::string error, int offset = -1) {
        if (offset < 0) {
            offset = position;
        }
        std::cout << infix << "\n";
        std::cout << std::string(offset, ' ');
        std::cout << "^ " << error << std::endl;
    }

public:
    Lexer(const std::string& input) : infix(input) {
        position = -1;
        rank = 0;
    }

    std::vector<Token> tokenize() {
        std::vector<Token> tokens;
        advance(); // not checking for exception here because empty input is
                   // ignored by the input loop
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
                bracketPositions.push(position);
            } else if (currentChar == ')') {
                if (bracketPositions.empty()) {
                    // no open bracket is set for this to close
                    reportError("missing opening parentheses");
                    return std::vector<Token>{};
                }
                tokens.push_back(RPARENToken());
                bracketPositions.pop();
            } else if (isDigit(currentChar) || isDecimal(currentChar)) {
                auto numberTokenResult = makeNumberToken();
                if (!numberTokenResult.success) {
                    return std::vector<Token>{};
                }
                tokens.push_back(numberTokenResult.token);
            } else {
                reportError("invalid character");
                return std::vector<Token>{};
            }

            {
                // * new scope to allow control transfer from goto
                auto lastTokenType = tokens.back().getTokenType();
                auto isNumber = lastTokenType == TokenType::NUMBER;
                auto isParen = lastTokenType == TokenType::LPAREN ||
                               lastTokenType == TokenType::RPAREN;
                if (isNumber) {
                    rank++;
                } else if (!isParen) {
                    rank--;
                }

                if (rank < 0) {
                    reportError("missing operand");
                    return std::vector<Token>{};
                } else if (rank > 1) {
                    reportError("missing operator");
                    return std::vector<Token>{};
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

        // rank checking is different here because the final value of an
        // expression must be equal to one. the range of rank is [0, 1] inside
        // loop.
        if (rank < 1) {
            reportError("missing operand");
            return std::vector<Token>{};
        } else if (rank > 1) {
            reportError("missing operator");
            return std::vector<Token>{};
        }

        if (!bracketPositions.empty()) {
            reportError("missing closing parentheses", bracketPositions.top());
            return std::vector<Token>{};
        }
        return tokens;
    }
};

/* * INTERPRETER * */
class Interpreter {
private:
    std::vector<Token> infixTokens;
    std::vector<Token> postfixTokens;

    std::stack<Token> operatorStack;

    void convertToPostfix() {
        for (auto& token : infixTokens) {
            auto tokenType = token.getTokenType();
            auto tokenPrecedence = token.getPrecedence();
            auto tokenAssoc = token.getAssociativity();

            if (tokenType == TokenType::NUMBER) {
                postfixTokens.push_back(token);
            } else {
                if (operatorStack.empty() || tokenType == TokenType::LPAREN) {
                    operatorStack.push(token);
                    continue;
                }
                if (tokenType == TokenType::RPAREN) {
                    while (operatorStack.top().getTokenType() !=
                           TokenType::LPAREN) {
                        postfixTokens.push_back(operatorStack.top());
                        operatorStack.pop();
                        // we need not worry about stack being empty because
                        // lparen is 100% present due to checking by lexer
                    }
                    operatorStack.pop(); // remove LPAREN from operator stack
                    continue;
                }
                auto topOperator = operatorStack.top();
                if (tokenPrecedence > topOperator.getPrecedence()) {
                    operatorStack.push(token);
                } else if (tokenPrecedence < topOperator.getPrecedence()) {
                    while (operatorStack.top().getPrecedence() >=
                           tokenPrecedence) {
                        postfixTokens.push_back(operatorStack.top());
                        operatorStack.pop();
                        if (operatorStack.empty()) {
                            break;
                        }
                    }
                    operatorStack.push(token);
                } else {
                    if (tokenAssoc == Associativity::LEFT) {
                        postfixTokens.push_back(operatorStack.top());
                        operatorStack.pop();
                        operatorStack.push(token);
                    } else {
                        operatorStack.push(token);
                    }
                }
            }
        }

        while (!operatorStack.empty()) {
            postfixTokens.push_back(operatorStack.top());
            operatorStack.pop();
        }
    }

    long double resolveOperator(TokenType tokenType,
                                std::stack<long double>& operands) {
        auto num2 = operands.top();
        operands.pop();
        auto num1 = operands.top();
        operands.pop();
        switch (tokenType) {
        case TokenType::PLUS_OP:
            return (num1 + num2);
            break;
        case TokenType::MINUS_OP:
            return (num1 - num2);
            break;

        case TokenType::MULTIPLY_OP:
            return (num1 * num2);
            break;
        case TokenType::DIVIDE_OP:
            return (num1 / num2);
            break;
        case TokenType::FLOOR_DIVIDE_OP:
            return (int(num1 / num2));
            break;

        case TokenType::EXPONENT_OP:
            return (pow(num1, num2));
            break;

        default:
            throw std::logic_error("unknown token type to resolve");
            break;
        }
    }

    double evalPostfix() {
        std::stack<long double> operands;
        for (auto token : postfixTokens) {
            auto tokenType = token.getTokenType();
            if (tokenType == TokenType::NUMBER) {
                operands.push(std::stold(token.getValue()));
            } else {
                operands.push(resolveOperator(tokenType, operands));
            }
        }
        return operands.top();
    }

public:
    Interpreter(std::vector<Token> tokens) : infixTokens(tokens) {
        convertToPostfix();
    };

    void getPostfix() {
        for (auto& tok : postfixTokens) {
            std::cout << tok.getValue() << " ";
        }
        std::cout << "\n";
    }

    std::string getInfix() {
        std::stack<std::string> expr;
        std::string e, e1, e2;
        for (auto& token : postfixTokens) {
            if (token.getTokenType() == TokenType::NUMBER) {
                expr.push(token.getValue());
            } else {
                e2 = expr.top();
                expr.pop();
                e1 = expr.top();
                expr.pop();
                e = "(" + e1 + " " + token.getValue() + " " + e2 + ")";
                expr.push(e);
            }
        }
        return expr.top();
    }

    double evaluate() { return evalPostfix(); }
};

/* * CRUSADE * */

int main(int argc, char const* argv[]) {
    auto debug = false;
    while (true) {
        std::string input;
        std::cout << "crusade > ";
        std::getline(std::cin, input);
        trim(input);

        if (input == "") {
            continue;
        }

        if (input == "quit" || input == "exit" || input == "/q") {
            std::cout << "bye" << std::endl;
            break;
        }

        if (input == "/debug") {
            debug = !debug;
            std::cout << "debug mode " << ((debug) ? "enabled" : "disabled")
                      << '\n';
            continue;
        }

        auto lexer = Lexer(input);
        auto tokens = lexer.tokenize();
        if (tokens.size() == 0) {
            // lexer encountered an error during tokenization, the expression is
            // invalid
            continue;
        }

        auto interpreter = Interpreter(tokens);
        if (debug) {
            std::cout << interpreter.getInfix() << "\n";
        }

        std::cout << interpreter.evaluate() << "\n";
    }

    return 0;
}