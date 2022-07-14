#pragma once
#include "lexer.hpp"
#include "ast.hpp"
#include <memory>

class Parser {
public:
    explicit Parser(Lexer& lexer);
    std::unique_ptr<AstNode> parse();

private:
    std::unique_ptr<AstNode> parseExpression();
    std::unique_ptr<AstNode> parseTerm();

    Lexer& lexer;
    Token currentToken;
    void advance();
};
