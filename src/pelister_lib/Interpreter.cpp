#include "Interpreter.hpp"
#include <stdexcept>
#include <iostream>
#include <cmath>

Interpreter::Interpreter() : memory(64 * 1024, 0.0) {
}

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

void Interpreter::rpush(double value) {
    return_stack.push_back(value);
}

double Interpreter::rpop() {
    if (return_stack.empty()) {
        throw std::runtime_error("Return stack underflow");
    }
    double value = return_stack.back();
    return_stack.pop_back();
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
        else if (auto doNode = dynamic_cast<const DoLoopNode*>(node.get())) {
            double start = pop();
            double limit = pop();

            for (long i = (long)start; i < (long)limit; ++i) {
                loop_indices.push_back(i); // Push current index for 'I' to access
                evaluate(doNode->getBody());
                loop_indices.pop_back(); // Pop index after iteration
            }
        }
        else if (auto defNode = dynamic_cast<FunctionDefinitionNode*>(node.get())) {
            dictionary[defNode->getName()] = const_cast<FunctionDefinitionNode*>(defNode)->releaseBody();
        }
        else if (auto wordNode = dynamic_cast<const WordNode*>(node.get())) {
            const auto& token = wordNode->getToken();

            auto it = dictionary.find(token.text);
            if (it != dictionary.end()) {
                evaluate(*(it->second));
                continue;
            }

            switch (token.type) {
                case TokenType::Plus: {
                    double b = pop(); double a = pop(); push(a + b); break;
                }
                case TokenType::Minus: {
                    double b = pop(); double a = pop(); push(a - b); break;
                }
                case TokenType::Multiply: {
                    double b = pop(); double a = pop(); push(a * b); break;
                }
                case TokenType::Divide: {
                    double b = pop(); double a = pop(); if (b == 0) throw std::runtime_error("Division by zero"); push(a / b); break;
                }
                case TokenType::Mod: {
                    double b = pop(); double a = pop(); push(fmod(a, b)); break;
                }
                case TokenType::Equals: {
                    double b = pop(); double a = pop(); push(a == b ? 1.0 : 0.0); break;
                }
                case TokenType::LessThan: {
                    double b = pop(); double a = pop(); push(a < b ? 1.0 : 0.0); break;
                }
                case TokenType::GreaterThan: {
                    double b = pop(); double a = pop(); push(a > b ? 1.0 : 0.0); break;
                }
                case TokenType::And: {
                    double b = pop(); double a = pop(); push(static_cast<double>((long)a & (long)b)); break;
                }
                case TokenType::Or: {
                    double b = pop(); double a = pop(); push(static_cast<double>((long)a | (long)b)); break;
                }
                case TokenType::Not: {
                    double a = pop(); push(a == 0.0 ? 1.0 : 0.0); break;
                }
                case TokenType::Dup: {
                    double a = pop(); push(a); push(a); break;
                }
                case TokenType::Drop: {
                    pop(); break;
                }
                case TokenType::Swap: {
                    double b = pop(); double a = pop(); push(b); push(a); break;
                }
                case TokenType::Over: {
                    double b = pop(); double a = pop(); push(a); push(b); push(a); break;
                }
                case TokenType::Rot: {
                    double c = pop(); double b = pop(); double a = pop(); push(b); push(c); push(a); break;
                }
                case TokenType::ToR: { // >R
                    rpush(pop());
                    break;
                }
                case TokenType::RFrom: { // R>
                    push(rpop());
                    break;
                }
                case TokenType::RFetch: { // R@
                    if (return_stack.empty()) {
                        throw std::runtime_error("Return stack underflow");
                    }
                    push(return_stack.back());
                    break;
                }
                case TokenType::Store: {
                    double addr = pop(); double val = pop(); if (addr < 0 || addr >= memory.size()) throw std::runtime_error("Memory access out of bounds"); memory[(size_t)addr] = val; break;
                }
                case TokenType::Fetch: {
                    double addr = pop(); if (addr < 0 || addr >= memory.size()) throw std::runtime_error("Memory access out of bounds"); push(memory[(size_t)addr]); break;
                }
                case TokenType::LoopIndexI: {
                    if (loop_indices.empty()) {
                        throw std::runtime_error("'I' can only be used inside a DO...LOOP");
                    }
                    push(loop_indices.back());
                    break;
                }
                case TokenType::LoopIndexJ: {
                    if (loop_indices.size() < 2) {
                        throw std::runtime_error("'J' can only be used inside nested DO...LOOPs");
                    }
                    push(loop_indices[loop_indices.size() - 2]);
                    break;
                }
                case TokenType::LoopIndexK: {
                    if (loop_indices.size() < 3) {
                        throw std::runtime_error("'K' can only be used inside triply-nested DO...LOOPs");
                    }
                    push(loop_indices[loop_indices.size() - 3]);
                    break;
                }
                case TokenType::Dot: {
                    std::cout << pop() << " "; break;
                }
                case TokenType::DotS: {
                                printStack();
                                break;
                            }
                case TokenType::Cr: {
                    std::cout << std::endl; break;
                }
                case TokenType::If: case TokenType::Else: case TokenType::Then:
                case TokenType::Colon: case TokenType::Semicolon:
                case TokenType::Do: case TokenType::Loop: {
                    throw std::runtime_error("Unexpected control flow word during execution: " + token.text);
                }
                default: {
                    throw std::runtime_error("Unknown word: " + token.text);
                }
            }
        }
    }
}
