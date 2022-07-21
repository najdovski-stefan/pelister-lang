#pragma once
#include "lexer.hpp"
#include "ast.hpp"
#include <memory>

class Parser {
public:
    explicit Parser(Lexer& lexer);
    std::unique_ptr<ProgramNode> parse();

private:
    std::unique_ptr<AstNode> parseStatement();
    std::unique_ptr<IfNode> parseIfStatement();

    Lexer& lexer;
    Token currentToken;
    void advance();
};
