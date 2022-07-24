#pragma once

#include "ast.hpp"
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>

class Interpreter {
public:
    void evaluate(const ProgramNode& ast);
    void printStack() const;
    const std::vector<double>& getStack() const;

private:
    void push(double value);
    double pop();

    std::vector<double> stack;
    std::unordered_map<std::string, std::unique_ptr<ProgramNode>> dictionary;
};
