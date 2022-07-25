#include "lexer.hpp"
#include <cctype>
#include <unordered_map>

static const std::unordered_map<std::string, TokenType> keywords = {
    {"+", TokenType::Plus},
    {"-", TokenType::Minus},
    {"*", TokenType::Multiply},
    {"/", TokenType::Divide},
    {"MOD", TokenType::Mod},
    {"/MOD", TokenType::ModSlash},
    {"DUP", TokenType::Dup},
    {"DROP", TokenType::Drop},
    {"SWAP", TokenType::Swap},
    {"OVER", TokenType::Over},
    {"ROT", TokenType::Rot},
    {">R", TokenType::ToR},
    {"R>", TokenType::RFrom},
    {"R@", TokenType::RFetch},
    {"=", TokenType::Equals},
    {"<", TokenType::LessThan},
    {">", TokenType::GreaterThan},
    {"AND", TokenType::And},
    {"OR", TokenType::Or},
    {"NOT", TokenType::Not},
    {"!", TokenType::Store},
    {"@", TokenType::Fetch},
    {":", TokenType::Colon},
    {";", TokenType::Semicolon},
    {"IF", TokenType::If},
    {"ELSE", TokenType::Else},
    {"THEN", TokenType::Then},
    {"DO", TokenType::Do},
    {"LOOP", TokenType::Loop},
    {"I", TokenType::LoopIndexI},
    {".", TokenType::Dot},
    {".S", TokenType::DotS},
    {"EMIT", TokenType::Emit},
    {"CR", TokenType::Cr}
};

bool is_double(const std::string& s) {
    if (s.empty()) return false;
    char* p;
    strtod(s.c_str(), &p);
    return (*p == 0);
}

Lexer::Lexer(std::string source) : source_text(std::move(source)), position(0) {}

Token Lexer::getNextToken() {
    while (position < source_text.length() && std::isspace(source_text[position])) {
        position++;
    }

    if (position >= source_text.length()) {
        return {TokenType::EndOfFile, ""};
    }

    if (source_text[position] == '(') {
        position++; // Consume the initial '('
        int nesting_level = 1;

        while (position < source_text.length() && nesting_level > 0) {
            if (source_text[position] == '(') {
                nesting_level++;
            } else if (source_text[position] == ')') {
                nesting_level--;
            }
            position++;
        }

        return getNextToken(); // Get the next real token
    }

    size_t start = position;
    while (position < source_text.length() && !std::isspace(source_text[position])) {
        position++;
    }

    std::string word = source_text.substr(start, position - start);

    auto it = keywords.find(word);
    if (it != keywords.end()) {
        return {it->second, word};
    }

    if (is_double(word)) {
        return {TokenType::Number, word};
    }

    return {TokenType::Word, word};
}
