#include <gtest/gtest.h>
#include "Interpreter.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"

// Helper function to run code through the whole pipeline
void run(Interpreter& interpreter, const std::string& code) {
    Lexer lexer(code);
    Parser parser(lexer);
    auto ast = parser.parse();
    interpreter.evaluate(*ast);
}

// Test fixture to have a fresh interpreter for each test
class InterpreterTest : public ::testing::Test {
protected:
    Interpreter interpreter;
};

TEST_F(InterpreterTest, BasicArithmetic) {
    run(interpreter, "10 5 +");
    ASSERT_NO_THROW({
        run(interpreter, ".");
    }); //print "15 " to stdout

    run(interpreter, "20 8 -");
    run(interpreter, "2 *");
    run(interpreter, "6 /");

    ASSERT_NO_THROW({
        run(interpreter, "."); //  4
    });
}

TEST_F(InterpreterTest, StackManipulation) {
    run(interpreter, "1 2 3"); // Stack: [1, 2, 3]
    run(interpreter, "DUP");   // Stack: [1, 2, 3, 3]
    run(interpreter, "DROP");  // Stack: [1, 2, 3]
    run(interpreter, "SWAP");  // Stack: [1, 3, 2]
    run(interpreter, "OVER");  // Stack: [1, 3, 2, 3]
    run(interpreter, "DROP DROP DROP DROP"); // Stack: []

    EXPECT_THROW(run(interpreter, "DROP"), std::runtime_error);
}

TEST_F(InterpreterTest, ComplexOperation) {
    //(5 DUP * ) = 25
    run(interpreter, "5 DUP *");
    run(interpreter, "25 -"); // 25 - 25 = 0
    ASSERT_NO_THROW({
        run(interpreter, "."); // Prints 0
    });
}

TEST_F(InterpreterTest, DivisionByZero) {
    run(interpreter, "10 0");
    EXPECT_THROW(run(interpreter, "/"), std::runtime_error);
}

TEST_F(InterpreterTest, StackUnderflow) {
    run(interpreter, "5");
    EXPECT_THROW(run(interpreter, "+"), std::runtime_error);
}
