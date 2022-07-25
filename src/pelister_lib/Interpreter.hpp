#pragma once

#include "ast.hpp"
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>

class Interpreter {
public:
    Interpreter();
    void evaluate(const ProgramNode& ast);
    void printStack() const;
    const std::vector<double>& getStack() const;
private:
    void push(double value);
    double pop();
    void rpush(double value);
    double rpop();

    std::vector<double> stack;
    std::unordered_map<std::string, std::unique_ptr<ProgramNode>> dictionary;
    std::vector<double> memory;
    std::vector<long> loop_indices;
    std::vector<double> return_stack;
};
