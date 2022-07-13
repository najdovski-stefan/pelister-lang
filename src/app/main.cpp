#include <iostream>
#include <string>
#include "lexer.hpp"
#include "parser.hpp"

int main() {
    std::string input = "123 + 45 - 6";
    std::cout << "Input string: " << input << std::endl;

    Lexer lexer(input);
    Parser parser(lexer);
    parser.parse();

    return 0;
}
