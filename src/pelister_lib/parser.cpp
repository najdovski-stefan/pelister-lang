#include "parser.hpp"
#include <iostream>

Parser::Parser(Lexer& lexer) : lexer(lexer) {}

void Parser::parse() {
    std::cout << "Parsing..." << std::endl;
    Token currentToken = lexer.getNextToken();
    while (currentToken.type != TokenType::EndOfFile) {
        tokens.push_back(currentToken);
        std::cout << "Parsed token: " << static_cast<int>(currentToken.type) << " (" << currentToken.text << ")" << std::endl;
        currentToken = lexer.getNextToken();
    }
    std::cout << "Parsing finished." << std::endl;
}

const std::vector<Token>& Parser::getTokens() const {
    return tokens;
}
