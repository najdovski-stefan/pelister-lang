#pragma once

#include "ast.hpp"
#include <vector>

class Interpreter {
public:
    void evaluate(const ProgramNode& ast);
    void printStack() const;
    const std::vector<double>& getStack() const;

private:
    void push(double value);
    double pop();

    std::vector<double> stack;
};
