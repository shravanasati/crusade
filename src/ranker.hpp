#ifndef RANKER_HPP
#define RANKER_HPP

#include "tokens.hpp"
#include <vector>
#include <string>

struct RankResult {
    bool success;
    std::string message;
};

class Ranker {
private:
    long rank;
    const std::vector<Token>& tokens;

public:
    Ranker(const std::vector<Token>& tokens);

    RankResult evaluate();
};

#endif // RANKER_HPP
