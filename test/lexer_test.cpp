#include <gtest/gtest.h>
#include "lexer.hpp"

void verify_token(Lexer& lexer, TokenType expected_type, const std::string& expected_text) {
    Token token = lexer.getNextToken();
    ASSERT_EQ(token.type, expected_type);
    ASSERT_EQ(token.text, expected_text);
}

TEST(LexerTest, HandlesSimpleNumbersAndOperators) {
    std::string input = "10 + 25";
    Lexer lexer(input);
    verify_token(lexer, TokenType::Number, "10");
    verify_token(lexer, TokenType::Plus, "+");
    verify_token(lexer, TokenType::Number, "25");
    verify_token(lexer, TokenType::EndOfFile, "");
}

TEST(LexerTest, HandlesForthCoreWordsAndComments) {
    std::string input = ": SQUARE ( n -- n*n ) DUP * ; 10 SQUARE .";
    Lexer lexer(input);
    verify_token(lexer, TokenType::Colon, ":");
    verify_token(lexer, TokenType::Word, "SQUARE");
    verify_token(lexer, TokenType::Dup, "DUP");
    verify_token(lexer, TokenType::Multiply, "*");
    verify_token(lexer, TokenType::Semicolon, ";");
    verify_token(lexer, TokenType::Number, "10");
    verify_token(lexer, TokenType::Word, "SQUARE");
    verify_token(lexer, TokenType::Dot, ".");
    verify_token(lexer, TokenType::EndOfFile, "");
}

TEST(LexerTest, HandlesNegativeNumbers) {
    std::string input = "-5 10 +";
    Lexer lexer(input);
    verify_token(lexer, TokenType::Number, "-5");
    verify_token(lexer, TokenType::Number, "10");
    verify_token(lexer, TokenType::Plus, "+");
    verify_token(lexer, TokenType::EndOfFile, "");
}

TEST(LexerTest, HandlesEmptyInput) {
    std::string input = "";
    Lexer lexer(input);
    verify_token(lexer, TokenType::EndOfFile, "");
}

TEST(LexerTest, HandlesOnlyWhitespace) {
    std::string input = "   \t\n  ";
    Lexer lexer(input);
    verify_token(lexer, TokenType::EndOfFile, "");
}

TEST(LexerTest, IgnoresUnclosedCommentAtEnd) {
    std::string input = "1 2 ( unfinished comment";
    Lexer lexer(input);
    verify_token(lexer, TokenType::Number, "1");
    verify_token(lexer, TokenType::Number, "2");
    verify_token(lexer, TokenType::EndOfFile, "");
}

TEST(LexerTest, HandlesMixedCaseKeywords) {
    std::string input = "dup DrOp";
    Lexer lexer(input);
    verify_token(lexer, TokenType::Word, "dup");
    verify_token(lexer, TokenType::Word, "DrOp");
    verify_token(lexer, TokenType::EndOfFile, "");
}
