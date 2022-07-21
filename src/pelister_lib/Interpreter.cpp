#include "Interpreter.hpp"
#include <stdexcept>
#include <iostream>

void Interpreter::push(double value) {
    stack.push_back(value);
}

double Interpreter::pop() {
    if (stack.empty()) {
        throw std::runtime_error("Stack underflow");
    }
    double value = stack.back();
    stack.pop_back();
    return value;
}

const std::vector<double>& Interpreter::getStack() const {
    return stack;
}

void Interpreter::printStack() const {
    std::cout << "<stack bottom> ";
    for (double val : stack) {
        std::cout << val << " ";
    }
    std::cout << "<top>" << std::endl;
}

void Interpreter::evaluate(const ProgramNode& ast) {
    for (const auto& node : ast.getNodes()) {
        if (auto numNode = dynamic_cast<const NumberNode*>(node.get())) {
            push(numNode->getValue());
        }
        else if (auto ifNode = dynamic_cast<const IfNode*>(node.get())) {
            double condition = pop();
            if (condition != 0.0) {
                evaluate(ifNode->getTrueBranch());
            } else if (ifNode->hasFalseBranch()) {
                evaluate(ifNode->getFalseBranch());
            }
        }
        else if (auto wordNode = dynamic_cast<const WordNode*>(node.get())) {
            const auto& token = wordNode->getToken();
            switch (token.type) {
                case TokenType::Plus: {
                    double b = pop();
                    double a = pop();
                    push(a + b);
                    break;
                }
                case TokenType::Minus: {
                    double b = pop();
                    double a = pop();
                    push(a - b);
                    break;
                }
                case TokenType::Multiply: {
                    double b = pop();
                    double a = pop();
                    push(a * b);
                    break;
                }
                case TokenType::Divide: {
                    double b = pop();
                    double a = pop();
                    if (b == 0) {
                        throw std::runtime_error("Division by zero");
                    }
                    push(a / b);
                    break;
                }
                case TokenType::Equals: {
                    double b = pop();
                    double a = pop();
                    push(a == b ? 1.0 : 0.0);
                    break;
                }
                case TokenType::LessThan: {
                    double b = pop();
                    double a = pop();
                    push(a < b ? 1.0 : 0.0);
                    break;
                }
                case TokenType::GreaterThan: {
                    double b = pop();
                    double a = pop();
                    push(a > b ? 1.0 : 0.0);
                    break;
                }
                case TokenType::And: {
                    double b = pop();
                    double a = pop();
                    push(static_cast<double>((long)a & (long)b));
                    break;
                }
                case TokenType::Or: {
                    double b = pop();
                    double a = pop();
                    push(static_cast<double>((long)a | (long)b));
                    break;
                }
                case TokenType::Not: {
                    double a = pop();
                    push(a == 0.0 ? 1.0 : 0.0);
                    break;
                }
                case TokenType::Dup: {
                    double a = pop();
                    push(a);
                    push(a);
                    break;
                }
                case TokenType::Drop: {
                    pop();
                    break;
                }
                case TokenType::Swap: {
                    double b = pop();
                    double a = pop();
                    push(b);
                    push(a);
                    break;
                }
                case TokenType::Over: {
                    double b = pop();
                    double a = pop();
                    push(a);
                    push(b);
                    push(a);
                    break;
                }
                case TokenType::Rot: {
                    double c = pop();
                    double b = pop();
                    double a = pop();
                    push(b);
                    push(c);
                    push(a);
                    break;
                }
                case TokenType::Dot: {
                    std::cout << pop() << " ";
                    break;
                }
                case TokenType::Cr: {
                    std::cout << std::endl;
                    break;
                }
                default: {
                    throw std::runtime_error("Unknown word: " + token.text);
                }
            }
        }
    }
}
