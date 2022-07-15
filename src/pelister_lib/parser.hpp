#pragma once
#include "lexer.hpp"
#include "ast.hpp"
#include <memory>

class Parser {
public:
    explicit Parser(Lexer& lexer);
    std::unique_ptr<ProgramNode> parse();

private:
    Lexer& lexer;
    Token currentToken;
    void advance();
};
