#pragma once
#include "lexer.hpp"
#include <vector>

class Parser {
public:
    Parser(Lexer& lexer);
    void parse();
    const std::vector<Token>& getTokens() const;
private:
    Lexer& lexer;
    std::vector<Token> tokens;
};
