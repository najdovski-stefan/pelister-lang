#pragma once
#include <string>

enum class TokenType { Number, Plus, Minus, EndOfFile, Unknown };

struct Token {
    TokenType type;
    std::string text;
};

class Lexer {
public:
    Lexer(std::string source);
    Token getNextToken();
private:
    std::string source_text;
    size_t position;
};
