#include <iostream>
#include <string>
#include <vector>
#include <filesystem>

#include "lexer.hpp"
#include "parser.hpp"
#include "AstVisualizer.hpp"

bool find_option(int argc, char* argv[], const std::string& option) {
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == option) {
            return true;
        }
    }
    return false;
}

int main(int argc, char* argv[]) {
    std::string input = "10 5 DUP * -";
    std::cout << "Input string: \"" << input << "\"" << std::endl;

    Lexer lexer(input);
    Parser parser(lexer);

    try {
        auto ast_root = parser.parse();

        if (find_option(argc, argv, "--ppm_gen_ast_tree")) {
            std::cout << "AST generation flag detected." << std::endl;

            std::filesystem::path exe_path(argv[0]);
            std::filesystem::path output_path = exe_path.parent_path() / "ast.dot";

            AstVisualizer visualizer;
            visualizer.generateDot(*ast_root, output_path.string());

            std::cout << "\nTo generate an image, install graphviz and run:" << std::endl;
            std::cout << "dot -Tpng " << output_path.string() << " -o ast.png" << std::endl;

            return 0;
        }

        std::cout << "Parser finished successfully. (No evaluation step yet)" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Parser Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
