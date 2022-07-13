#include <gtest/gtest.h>
#include "lexer.hpp"

TEST(LexerTest, HandlesNumbersAndOperators) {
    std::string input = "10 + 25";
    Lexer lexer(input);

    Token token1 = lexer.getNextToken();
    ASSERT_EQ(token1.type, TokenType::Number);
    ASSERT_EQ(token1.text, "10");

    Token token2 = lexer.getNextToken();
    ASSERT_EQ(token2.type, TokenType::Plus);
    ASSERT_EQ(token2.text, "+");

    Token token3 = lexer.getNextToken();
    ASSERT_EQ(token3.type, TokenType::Number);
    ASSERT_EQ(token3.text, "25");

    Token token4 = lexer.getNextToken();
    ASSERT_EQ(token4.type, TokenType::EndOfFile);
}
