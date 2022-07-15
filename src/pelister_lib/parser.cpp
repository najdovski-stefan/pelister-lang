#include "parser.hpp"

Parser::Parser(Lexer& lexer) : lexer(lexer), currentToken({TokenType::Unknown, ""}) {
    advance();
}

void Parser::advance() {
    currentToken = lexer.getNextToken();
}

std::unique_ptr<ProgramNode> Parser::parse() {
    auto programNode = std::make_unique<ProgramNode>();

    while (currentToken.type != TokenType::EndOfFile) {
        if (currentToken.type == TokenType::Number) {
            double value = std::stod(currentToken.text);
            programNode->addNode(std::make_unique<NumberNode>(value));
        } else {
            programNode->addNode(std::make_unique<WordNode>(currentToken));
        }
        advance();
    }
    return programNode;
}
