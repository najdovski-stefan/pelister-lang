#include "lexer.hpp"
#include <cctype>

Lexer::Lexer(std::string source) : source_text(std::move(source)), position(0) {}

Token Lexer::getNextToken() {
    // Skip whitespace
    while (position < source_text.length() && std::isspace(source_text[position])) {
        position++;
    }

    if (position >= source_text.length()) {
        return {TokenType::EndOfFile, ""};
    }

    char current_char = source_text[position];

    if (std::isdigit(current_char)) {
        size_t start = position;
        while (position < source_text.length() && std::isdigit(source_text[position])) {
            position++;
        }
        return {TokenType::Number, source_text.substr(start, position - start)};
    }

    if (current_char == '+') {
        position++;
        return {TokenType::Plus, "+"};
    }

    if (current_char == '-') {
        position++;
        return {TokenType::Minus, "-"};
    }

    position++;
    return {TokenType::Unknown, std::string(1, current_char)};
}
