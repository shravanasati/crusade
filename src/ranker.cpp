#include "lexer.hpp"
#include "tokens.hpp"

struct RankResult {
    bool success;
    std::string message;
};

class Ranker {
private:
    long rank;
    const std::vector<Token>& tokens;

public:
    Ranker(const std::vector<Token>& tokens) : tokens(tokens) {}

    RankResult evaluate() {
        std::vector<TokenType> operators = {
            TokenType::PLUS_OP,         TokenType::MINUS_OP,
            TokenType::MULTIPLY_OP,     TokenType::DIVIDE_OP,
            TokenType::FLOOR_DIVIDE_OP, TokenType::EXPONENT_OP,
            TokenType::LPAREN,          TokenType::RPAREN};
        for (auto token : tokens) {
            bool isOperator = false;
            for (auto op : operators) {
                if (token.getTokenType() == op) {
                    rank--;
                    isOperator = true;
                    break;
                }
            }
            if (!isOperator) {
                // its a number token
                rank++;
            }
        }

        auto success = rank == 1;
        auto message = "success";
        if (rank <= 0) {
            message = "missing operands";
        } else if (rank > 1) {
            message = "missing operators";
        }

        return RankResult{success, message};
    }
};