#pragma once
#include <string>


enum class TokenType {
    // Core Types
    Number,
    Word,

    // Arithmetic
    Plus, Minus, Multiply, Divide, Mod, ModSlash,

    // Stack Manipulation
    Dup, Drop, Swap, Over, Rot,

    // Comparison
    Equals, LessThan, GreaterThan,

    // Logical
    And, Or, Not,

    // Memory / Variables
    Store, Fetch, // ! and @

    // Defining Words
    Colon, Semicolon,

    // Control Flow
    If, Else, Then, Do, Loop,

    // I/O
    Dot, Emit, Cr,

    // Special / End
    LeftParen,
    EndOfFile,
    Unknown
};

struct Token {
    TokenType type;
    std::string text;
};

class Lexer {
public:
    explicit Lexer(std::string source);
    Token getNextToken();
private:
    std::string source_text;
    size_t position;
};
