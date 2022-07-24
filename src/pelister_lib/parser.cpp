#include "parser.hpp"
#include <stdexcept>

Parser::Parser(Lexer& lexer) : lexer(lexer), currentToken({TokenType::Unknown, ""}) {
    advance();
}
void Parser::advance() { currentToken = lexer.getNextToken(); }

std::unique_ptr<ProgramNode> Parser::parse() {
    auto programNode = std::make_unique<ProgramNode>();
    while (currentToken.type != TokenType::EndOfFile) {
        programNode->addNode(parseStatement());
    }
    return programNode;
}

std::unique_ptr<AstNode> Parser::parseStatement() {
    if (currentToken.type == TokenType::If) return parseIfStatement();
    if (currentToken.type == TokenType::Colon) return parseFunctionDefinition();

    std::unique_ptr<AstNode> node;
    if (currentToken.type == TokenType::Number) {
        node = std::make_unique<NumberNode>(std::stod(currentToken.text));
    } else {
        node = std::make_unique<WordNode>(currentToken);
    }
    advance();
    return node;
}

std::unique_ptr<FunctionDefinitionNode> Parser::parseFunctionDefinition() {
    advance(); // Consume ':'
    if (currentToken.type != TokenType::Word) {
        throw std::runtime_error("Expected function name after ':'");
    }
    std::string name = currentToken.text;
    advance(); // Consume function name

    auto body = std::make_unique<ProgramNode>();
    while (currentToken.type != TokenType::Semicolon) {
        if (currentToken.type == TokenType::EndOfFile) {
            throw std::runtime_error("Unterminated function definition; missing ';'");
        }
        body->addNode(parseStatement());
    }
    advance(); // Consume ';'
    return std::make_unique<FunctionDefinitionNode>(name, std::move(body));
}

std::unique_ptr<IfNode> Parser::parseIfStatement() {
    advance();
    auto true_branch = std::make_unique<ProgramNode>();
    while (currentToken.type != TokenType::Else && currentToken.type != TokenType::Then) {
        if (currentToken.type == TokenType::EndOfFile) throw std::runtime_error("Unterminated IF; missing THEN");
        true_branch->addNode(parseStatement());
    }
    std::unique_ptr<ProgramNode> false_branch = nullptr;
    if (currentToken.type == TokenType::Else) {
        advance();
        false_branch = std::make_unique<ProgramNode>();
        while (currentToken.type != TokenType::Then) {
            if (currentToken.type == TokenType::EndOfFile) throw std::runtime_error("Unterminated ELSE; missing THEN");
            false_branch->addNode(parseStatement());
        }
    }
    if (currentToken.type != TokenType::Then) throw std::runtime_error("Expected THEN to close IF");
    advance();
    if (!false_branch) false_branch = std::make_unique<ProgramNode>();
    return std::make_unique<IfNode>(std::move(true_branch), std::move(false_branch));
}
