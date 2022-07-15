#pragma once

#include "lexer.hpp"
#include <memory>
#include <string>
#include <utility>
#include <vector>

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

class WordNode : public AstNode {
public:
    explicit WordNode(Token token) : token(std::move(token)) {}
    std::string toString() const override { return token.text; }
    const Token& getToken() const { return token; }
private:
    Token token;
};

class ProgramNode : public AstNode {
public:
    std::string toString() const override { return "Program"; }
    void addNode(std::unique_ptr<AstNode> node) {
        nodes.push_back(std::move(node));
    }
    const std::vector<std::unique_ptr<AstNode>>& getNodes() const {
        return nodes;
    }
private:
    std::vector<std::unique_ptr<AstNode>> nodes;
};
