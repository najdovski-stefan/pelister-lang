#pragma once

#include "lexer.hpp"
#include <memory>
#include <string>
#include <utility>

class AstNode {
public:
    virtual ~AstNode() = default;
    virtual std::string toString() const = 0;
};

class NumberNode : public AstNode {
public:
    explicit NumberNode(double value) : value(value) {}
    std::string toString() const override { return std::to_string(value); }
    double getValue() const { return value; }
private:
    double value;
};

class BinaryOpNode : public AstNode {
public:
    BinaryOpNode(Token op, std::unique_ptr<AstNode> left, std::unique_ptr<AstNode> right)
        : op(std::move(op)), left(std::move(left)), right(std::move(right)) {}

    std::string toString() const override { return op.text; }
    const AstNode& getLeft() const { return *left; }
    const AstNode& getRight() const { return *right; }

private:
    Token op;
    std::unique_ptr<AstNode> left;
    std::unique_ptr<AstNode> right;
};
