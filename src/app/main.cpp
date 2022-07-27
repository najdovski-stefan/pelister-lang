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

    while ((line_c = linenoise("> ")) != nullptr) {
        std::string line(line_c);
        free(line_c);

        if (line == "bye") break;
        if (line.empty()) continue;

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

void runFile(const std::string& filepath, const std::string& vizPath) {
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

         if (!vizPath.empty()) {
            AstVisualizer visualizer;
            visualizer.generateDot(*ast, vizPath);
        }

        interpreter.evaluate(*ast);
        std::cout << "Program finished. Final stack state:" << std::endl;
        interpreter.printStack();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void printUsage(const char* program_name) {
    std::cout << "Usage: " << program_name << " [options] [filepath]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  --repl                Enter interactive REPL mode." << std::endl;
    std::cout << "  --visualize <path>    Generate an AST visualization .dot file at <path>." << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printUsage(argv[0]);
        return 1;
    }

    std::string filepath;
    std::string vizPath;
    bool replMode = false;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--repl") {
            replMode = true;
        } else if (arg == "--visualize") {
            if (i + 1 < argc) {
                vizPath = argv[++i];
            } else {
                std::cerr << "Error: --visualize requires a path argument." << std::endl;
                return 1;
            }
        } else if (arg.rfind("--", 0) != 0) {
            filepath = arg;
        } else {
            std::cerr << "Error: Unknown flag '" << arg << "'" << std::endl;
            printUsage(argv[0]);
            return 1;
        }
    }

    if (replMode) {
        runRepl();
    } else if (!filepath.empty()) {
        runFile(filepath, vizPath);
    } else {
        printUsage(argv[0]);
    }

    return 0;
}
