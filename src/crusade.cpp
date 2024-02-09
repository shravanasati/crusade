#include <iostream>
#include "stringutils.hpp"
#include "lexer.hpp"
#include "ranker.hpp"

int main(int argc, char const* argv[]) {
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
        
		auto lexer = Lexer(input);
		auto tokens = lexer.tokenize();
		if (tokens.size() == 0) {
			// lexer encountered an error during tokenization
			continue;
		}

		auto ranker = Ranker(tokens);
		auto rankResult = ranker.evaluate();
		if (!rankResult.success) {
			std::cout << rankResult.message << "\n";
			continue;
		}

    }

    return 0;
}