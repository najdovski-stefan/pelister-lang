#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include "lexer.hpp"
#include "parser.hpp"
#include "AstVisualizer.hpp"
#include "Interpreter.hpp"
#include "linenoise.h"

void runRepl() {
    Interpreter interpreter;
    std::cout << "Pelister-Lang REPL v1.0. Type 'bye' or press Ctrl-D to exit." << std::endl;

    linenoiseHistoryLoad("history.txt");

    char* line_c;
    const char* prompt = "> ";

    while ((line_c = linenoise(prompt)) != nullptr) {
        std::string line(line_c);
        free(line_c); // linenoise free

        if (line == "bye") {
            break;
        }

        if (line.empty()) {
            continue;
        }

        linenoiseHistoryAdd(line.c_str());
        linenoiseHistorySave("history.txt");

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

void runFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file '" << filepath << "'" << std::endl;
        return;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string source_code = buffer.str();

    try {
        Lexer lexer(source_code);
        Parser parser(lexer);
        Interpreter interpreter;
        auto ast = parser.parse();
        interpreter.evaluate(*ast);
        std::cout << "Program finished. Final stack state:" << std::endl;
        interpreter.printStack();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void printUsage(const char* program_name) {
    std::cout << "Usage: " << program_name << " [filepath] | --repl" << std::endl;
    std::cout << "  [filepath]   Execute a .peli script file." << std::endl;
    std::cout << "  --repl         Enter interactive REPL mode." << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printUsage(argv[0]);
        return 1;
    }

    std::string argument = argv[1];

    if (argument == "--repl") {
        runRepl();
    } else if (argument.rfind("--", 0) != 0) { // Check if it's NOT a flag
        runFile(argument);
    } else {
        std::cerr << "Error: Unknown flag '" << argument << "'" << std::endl;
        printUsage(argv[0]);
        return 1;
    }

    return 0;
}
