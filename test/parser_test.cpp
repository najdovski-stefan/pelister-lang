#include <gtest/gtest.h>
#include "lexer.hpp"
#include "parser.hpp"
#include "ast.hpp"
TEST(ParserTest, ConstructsCorrectAstForSimpleExpression) {
    std::string input = "5 - 2";
    Lexer lexer(input);
    Parser parser(lexer);

    std::unique_ptr<AstNode> ast_root = parser.parse();
    ASSERT_NE(ast_root, nullptr);

    auto* root_node = dynamic_cast<BinaryOpNode*>(ast_root.get());
    ASSERT_NE(root_node, nullptr) << "Root node should be a BinaryOpNode for '5 - 2'";

    EXPECT_EQ(root_node->toString(), "-");

    const auto& left_child = root_node->getLeft();
    auto* left_num = dynamic_cast<const NumberNode*>(&left_child);
    ASSERT_NE(left_num, nullptr) << "Left child should be a NumberNode";
    EXPECT_EQ(left_num->getValue(), 5.0);

    const auto& right_child = root_node->getRight();
    auto* right_num = dynamic_cast<const NumberNode*>(&right_child);
    ASSERT_NE(right_num, nullptr) << "Right child should be a NumberNode";
    EXPECT_EQ(right_num->getValue(), 2.0);
}
