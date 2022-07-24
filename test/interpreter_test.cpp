#include <gtest/gtest.h>
#include "Interpreter.hpp"
#include "lexer.hpp"
#include "parser.hpp"

void run(Interpreter& interpreter, const std::string& code) {
    Lexer lexer(code);
    Parser parser(lexer);
    auto ast = parser.parse();
    interpreter.evaluate(*ast);
}

class InterpreterTest : public ::testing::Test {
protected:
    Interpreter interpreter;
};

TEST_F(InterpreterTest, BasicArithmetic) {
    run(interpreter, "10 5 +");
    run(interpreter, "20 8 -");
    run(interpreter, "2 *");
    run(interpreter, "6 /");
}

TEST_F(InterpreterTest, StackManipulation) {
    run(interpreter, "1 2 3");
    run(interpreter, "DUP");
    run(interpreter, "DROP");
    run(interpreter, "SWAP");
    run(interpreter, "OVER");
    run(interpreter, "DROP DROP DROP DROP");
    EXPECT_THROW(run(interpreter, "DROP"), std::runtime_error);
}

TEST_F(InterpreterTest, ComplexOperation) {
    run(interpreter, "5 DUP *");
    run(interpreter, "25 -");
}

TEST_F(InterpreterTest, DivisionByZero) {
    run(interpreter, "10 0");
    EXPECT_THROW(run(interpreter, "/"), std::runtime_error);
}

TEST_F(InterpreterTest, StackUnderflowForBinaryOperator) {
    run(interpreter, "5");
    EXPECT_THROW(run(interpreter, "+"), std::runtime_error);
}

TEST_F(InterpreterTest, StackUnderflowForUnaryOperator) {
    EXPECT_THROW(run(interpreter, "DUP"), std::runtime_error);
    EXPECT_THROW(run(interpreter, "DROP"), std::runtime_error);
    EXPECT_THROW(run(interpreter, "."), std::runtime_error);
}

TEST_F(InterpreterTest, OverOperationRequiresTwoItems) {
    run(interpreter, "1");
    EXPECT_THROW(run(interpreter, "OVER"), std::runtime_error);
}

TEST_F(InterpreterTest, DotCommandPrintsAndConsumes) {
    std::stringstream buffer;
    std::streambuf* old_cout = std::cout.rdbuf(buffer.rdbuf());

    run(interpreter, "123 456");
    run(interpreter, ".");
    EXPECT_EQ(buffer.str(), "456 ");
    buffer.str("");

    run(interpreter, ".");
    EXPECT_EQ(buffer.str(), "123 ");

    EXPECT_THROW(run(interpreter, "."), std::runtime_error);

    std::cout.rdbuf(old_cout);
}

TEST_F(InterpreterTest, RotOperation) {
    run(interpreter, "1 2 3");
    run(interpreter, "ROT");
}

TEST_F(InterpreterTest, LogicalOperations) {
    run(interpreter, "1 1 AND");
    EXPECT_EQ(interpreter.getStack().back(), 1.0);
    run(interpreter, "1 0 AND");
    EXPECT_EQ(interpreter.getStack().back(), 0.0);

    run(interpreter, "7 3 AND"); // 0111 & 0011 = 0011 (3)
    EXPECT_EQ(interpreter.getStack().back(), 3.0);

    run(interpreter, "1 0 OR");
    EXPECT_EQ(interpreter.getStack().back(), 1.0);
    run(interpreter, "5 2 OR"); // 0101 | 0010 = 0111 (7)
    EXPECT_EQ(interpreter.getStack().back(), 7.0);

    run(interpreter, "1 NOT");
    EXPECT_EQ(interpreter.getStack().back(), 0.0);
    run(interpreter, "0 NOT");
    EXPECT_EQ(interpreter.getStack().back(), 1.0);
    run(interpreter, "-1 NOT");
    EXPECT_EQ(interpreter.getStack().back(), 0.0);
}

TEST_F(InterpreterTest, ComparisonOperations) {
    run(interpreter, "5 5 =");
    EXPECT_EQ(interpreter.getStack().back(), 1.0);
    run(interpreter, "5 4 =");
    EXPECT_EQ(interpreter.getStack().back(), 0.0);

    run(interpreter, "4 5 <");
    EXPECT_EQ(interpreter.getStack().back(), 1.0);
    run(interpreter, "5 4 <");
    EXPECT_EQ(interpreter.getStack().back(), 0.0);

    run(interpreter, "5 4 >");
    EXPECT_EQ(interpreter.getStack().back(), 1.0);
    run(interpreter, "4 5 >");
    EXPECT_EQ(interpreter.getStack().back(), 0.0);
}


TEST_F(InterpreterTest, ControlFlowIfThen) {
    run(interpreter, "1 IF 100 THEN");
    EXPECT_EQ(interpreter.getStack().back(), 100.0);
    run(interpreter, "0 IF 200 THEN");
    EXPECT_EQ(interpreter.getStack().size(), 1); // no change
}

TEST_F(InterpreterTest, ControlFlowIfElseThen) {
    run(interpreter, "1 IF 100 ELSE 200 THEN");
    EXPECT_EQ(interpreter.getStack().back(), 100.0);
    run(interpreter, "0 IF 100 ELSE 200 THEN");
    EXPECT_EQ(interpreter.getStack().back(), 200.0);
}

TEST_F(InterpreterTest, CombinedLogicAndControlFlow) {
    run(interpreter, "10 5 > IF 100 ELSE 200 THEN 5 +"); // 100 + 5
    EXPECT_EQ(interpreter.getStack().back(), 105.0);

    run(interpreter, "10 20 > IF 100 ELSE 200 THEN 5 +"); // 200 + 5
    EXPECT_EQ(interpreter.getStack().back(), 205.0);
}

TEST_F(InterpreterTest, NestedControlFlow) {
    run(interpreter, "1 IF 1 IF 100 THEN 50 + THEN");
    const auto& stack = interpreter.getStack();
    ASSERT_EQ(stack.size(), 1);
    EXPECT_EQ(stack[0], 150.0);
}

TEST_F(InterpreterTest, ParserThrowsOnUnterminatedIf) {
    EXPECT_THROW(run(interpreter, "1 IF 100"), std::runtime_error);
    EXPECT_THROW(run(interpreter, "1 IF 100 ELSE 200"), std::runtime_error);
}
TEST_F(InterpreterTest, SimpleFunctionDefinitionAndCall) {
    run(interpreter, ": DOUBLE 2 * ; 10 DOUBLE");
    const auto& stack = interpreter.getStack();
    ASSERT_EQ(stack.size(), 1);
    EXPECT_EQ(stack[0], 20.0);
}

TEST_F(InterpreterTest, FunctionCallingAnotherFunction) {
    run(interpreter, ": DOUBLE 2 * ; : QUADRUPLE DOUBLE DOUBLE ; 5 QUADRUPLE");
    const auto& stack = interpreter.getStack();
    ASSERT_EQ(stack.size(), 1);
    EXPECT_EQ(stack[0], 20.0);
}

TEST_F(InterpreterTest, FunctionWithLogicAndControlFlow) {
    run(interpreter, ": ABS DUP 0 < IF -1 * THEN ;");

    run(interpreter, "-10 ABS");
    EXPECT_EQ(interpreter.getStack().back(), 10.0);
    run(interpreter, "DROP");

    run(interpreter, "15 ABS");
    EXPECT_EQ(interpreter.getStack().back(), 15.0);
    run(interpreter, "DROP");

    run(interpreter, "0 ABS");
    EXPECT_EQ(interpreter.getStack().back(), 0.0);
}

TEST_F(InterpreterTest, CallingUndefinedFunctionThrows) {
    EXPECT_THROW(run(interpreter, "UNDEFINED-WORD"), std::runtime_error);
}

TEST_F(InterpreterTest, ParserThrowsOnUnterminatedFunction) {
    EXPECT_THROW(run(interpreter, ": BROKEN 1 2 +"), std::runtime_error);
}
