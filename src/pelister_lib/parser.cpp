#include "parser.hpp"
#include <stdexcept>

Parser::Parser(Lexer& lexer) : lexer(lexer), currentToken({TokenType::Unknown, ""}) {
    advance();
}

void Parser::advance() {
    currentToken = lexer.getNextToken();
}

std::unique_ptr<AstNode> Parser::parse() {
    auto result = parseExpression();
    if (currentToken.type != TokenType::EndOfFile) {
        throw std::runtime_error("Unexpected token after expression");
    }
    return result;
}

std::unique_ptr<AstNode> Parser::parseTerm() {
    if (currentToken.type == TokenType::Number) {
        double value = std::stod(currentToken.text);
        advance();
        return std::make_unique<NumberNode>(value);
    }
    throw std::runtime_error("Unexpected token: expected a number");
}
std::unique_ptr<AstNode> Parser::parseExpression() {
    auto left = parseTerm();

    while (currentToken.type == TokenType::Plus || currentToken.type == TokenType::Minus) {
        Token op = currentToken;
        advance();
        auto right = parseTerm();
        left = std::make_unique<BinaryOpNode>(op, std::move(left), std::move(right));
    }

    return left;
}
