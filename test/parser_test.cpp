#include <gtest/gtest.h>
#include "lexer.hpp"
#include "parser.hpp"

TEST(ParserTest, CorrectlyTokenizesInput) {
    std::string input = "5 - 2";
    Lexer lexer(input);
    Parser parser(lexer);

    parser.parse();

    const auto& tokens = parser.getTokens();
    ASSERT_EQ(tokens.size(), 3);
    EXPECT_EQ(tokens[0].type, TokenType::Number);
    EXPECT_EQ(tokens[0].text, "5");
    EXPECT_EQ(tokens[1].type, TokenType::Minus);
    EXPECT_EQ(tokens[2].type, TokenType::Number);
    EXPECT_EQ(tokens[2].text, "2");
}
