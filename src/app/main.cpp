#include <iostream>
#include <string>
#include <vector>
#include <filesystem>

#include "lexer.hpp"
#include "parser.hpp"
#include "AstVisualizer.hpp"
#include "Interpreter.hpp"

bool find_option(int argc, char* argv[], const std::string& option) {
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == option) {
            return true;
        }
    }
    return false;
}

void runRepl() {
    Interpreter interpreter;
    std::cout << "Pelister-Lang REPL v1.0. Type 'bye' to exit." << std::endl;

    for (;;) {
        std::cout << "> ";
        std::string line;
        if (!std::getline(std::cin, line) || line == "bye") {
            break;
        }

        if (line.empty()) {
            continue;
        }

        try {
            Lexer lexer(line);
            Parser parser(lexer);
            auto ast = parser.parse();
            interpreter.evaluate(*ast);
            interpreter.printStack();
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
}

void runSingle(const std::string& input, bool generate_ast, const std::string& exe_path_str) {
    Lexer lexer(input);
    Parser parser(lexer);
    Interpreter interpreter;

    try {
        auto ast_root = parser.parse();

        if (generate_ast) {
            std::cout << "AST generation flag detected." << std::endl;
            std::filesystem::path exe_path(exe_path_str);
            std::filesystem::path output_path = exe_path.parent_path() / "ast.dot";

            AstVisualizer visualizer;
            visualizer.generateDot(*ast_root, output_path.string());

            std::cout << "\nTo generate an image, run:" << std::endl;
            std::cout << "dot -Tpng " << output_path.string() << " -o ast.png" << std::endl;
            return;
        }

        interpreter.evaluate(*ast_root);
        std::cout << "Final stack state:" << std::endl;
        interpreter.printStack();

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

int main(int argc, char* argv[]) {
    if (find_option(argc, argv, "--repl")) {
        runRepl();
    } else {
        std::string input = "10 5 DUP * -";
        std::cout << "Input string: \"" << input << "\"" << std::endl;
        bool generate_ast = find_option(argc, argv, "--ppm_gen_ast_tree");
        runSingle(input, generate_ast, argv[0]);
    }

    return 0;
}
