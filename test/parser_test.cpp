#include <gtest/gtest.h>
#include "lexer.hpp"
#include "parser.hpp"
#include "ast.hpp"

TEST(ParserTest, ConstructsCorrectAstForForthSequence) {
    std::string input = "10 5 DUP";
    Lexer lexer(input);
    Parser parser(lexer);

    std::unique_ptr<ProgramNode> ast = parser.parse();
    ASSERT_NE(ast, nullptr);

    const auto& nodes = ast->getNodes();
    ASSERT_EQ(nodes.size(), 3);

    auto* node1 = dynamic_cast<NumberNode*>(nodes[0].get());
    ASSERT_NE(node1, nullptr);
    EXPECT_EQ(node1->getValue(), 10.0);

    auto* node2 = dynamic_cast<NumberNode*>(nodes[1].get());
    ASSERT_NE(node2, nullptr);
    EXPECT_EQ(node2->getValue(), 5.0);

    auto* node3 = dynamic_cast<WordNode*>(nodes[2].get());
    ASSERT_NE(node3, nullptr);
    EXPECT_EQ(node3->getToken().text, "DUP");
    EXPECT_EQ(node3->getToken().type, TokenType::Dup);
}
