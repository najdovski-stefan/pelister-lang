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

class IfNode : public AstNode {
public:
    IfNode(std::unique_ptr<ProgramNode> true_branch, std::unique_ptr<ProgramNode> false_branch)
        : true_branch(std::move(true_branch)), false_branch(std::move(false_branch)) {}
    std::string toString() const override { return "IF"; }
    const ProgramNode& getTrueBranch() const { return *true_branch; }
    const ProgramNode& getFalseBranch() const { return *false_branch; }
    bool hasFalseBranch() const { return false_branch != nullptr; }
private:
    std::unique_ptr<ProgramNode> true_branch;
    std::unique_ptr<ProgramNode> false_branch;
};

class FunctionDefinitionNode : public AstNode {
public:
    FunctionDefinitionNode(std::string name, std::unique_ptr<ProgramNode> body)
        : name(std::move(name)), body(std::move(body)) {}
    std::string toString() const override { return ":" + name; }
    const std::string& getName() const { return name; }
    const ProgramNode& getBody() const { return *body; }
    std::unique_ptr<ProgramNode> releaseBody() { return std::move(body); }
private:
    std::string name;
    std::unique_ptr<ProgramNode> body;
};

class DoLoopNode : public AstNode {
public:
    DoLoopNode(std::unique_ptr<ProgramNode> body) : body(std::move(body)) {}
    std::string toString() const override { return "DO-LOOP"; }
    const ProgramNode& getBody() const { return *body; }
    std::unique_ptr<ProgramNode> releaseBody() { return std::move(body); }
private:
    std::unique_ptr<ProgramNode> body;
};
