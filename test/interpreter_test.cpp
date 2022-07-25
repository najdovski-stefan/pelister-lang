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

TEST_F(InterpreterTest, MemoryOperations) {
    run(interpreter, "123 100 !"); // Store 123 at address 100
    run(interpreter, "100 @");    // Fetch from address 100
    const auto& stack = interpreter.getStack();
    ASSERT_EQ(stack.size(), 1);
    EXPECT_EQ(stack[0], 123.0);
    EXPECT_THROW(run(interpreter, "99999 !"), std::runtime_error); // Out of bounds
}

TEST_F(InterpreterTest, DoLoopOperation) {
    // 5 0 DO I + LOOP -> should calculate 0+1+2+3+4 = 10
    run(interpreter, "0 5 0 DO I + LOOP");
    const auto& stack = interpreter.getStack();
    ASSERT_EQ(stack.size(), 1);
    EXPECT_EQ(stack[0], 10.0);
}

TEST_F(InterpreterTest, NestedDoLoop) {
    run(interpreter, "2 0 DO 3 0 DO I LOOP LOOP");
    const auto& stack = interpreter.getStack();
    // The inner loop (0..2) runs, pushing 0 1 2.
    // The outer loop (0..1) runs twice.
    // So we expect 0 1 2 pushed, then 0 1 2 pushed again.
    ASSERT_EQ(stack.size(), 6);
    EXPECT_EQ(stack[0], 0.0);
    EXPECT_EQ(stack[1], 1.0);
    EXPECT_EQ(stack[2], 2.0);
    EXPECT_EQ(stack[3], 0.0);
    EXPECT_EQ(stack[4], 1.0);
    EXPECT_EQ(stack[5], 2.0);
}

TEST_F(InterpreterTest, LoopWithConditional) {
    // Sums even numbers from 0 to 9 (0+2+4+6+8 = 20)
    run(interpreter, "0 10 0 DO I DUP 2 MOD 0 = IF + ELSE DROP THEN LOOP");
    const auto& stack = interpreter.getStack();
    ASSERT_EQ(stack.size(), 1);
    EXPECT_EQ(stack[0], 20.0);
}

TEST_F(InterpreterTest, LoopWithComment) {
    // 5 0 DO I + LOOP -> should calculate 0+1+2+3+4 = 10
    // The comment should be ignored by the lexer and parser.
    run(interpreter, "0 5 0 DO ( this is a comment ) I + LOOP");
    const auto& stack = interpreter.getStack();
    ASSERT_EQ(stack.size(), 1);
    EXPECT_EQ(stack[0], 10.0);
}

TEST_F(InterpreterTest, FunctionWithLoop) {
    run(interpreter, ": SUM-UP-TO ( limit -- sum ) 0 SWAP 0 DO I + LOOP ;");
    run(interpreter, "5 SUM-UP-TO");
    const auto& stack = interpreter.getStack();
    ASSERT_EQ(stack.size(), 1);
    EXPECT_EQ(stack[0], 10.0); // 0+1+2+3+4
}

TEST_F(InterpreterTest, FunctionWithCommentAndLogic) {
    run(interpreter, ": IS-POSITIVE ( n -- boolean ) DUP 0 > ( check if n > 0 ) ;");

    run(interpreter, "10 IS-POSITIVE");
    EXPECT_EQ(interpreter.getStack().back(), 1.0);
    run(interpreter, "DROP");

    run(interpreter, "-5 IS-POSITIVE");
    EXPECT_EQ(interpreter.getStack().back(), 0.0);
    run(interpreter, "DROP");

    run(interpreter, "0 IS-POSITIVE");
    EXPECT_EQ(interpreter.getStack().back(), 0.0);
}


TEST_F(InterpreterTest, ComplexFunctionWithLoopAndConditional) {
    run(interpreter, ": SUM-EVENS ( limit -- sum ) 0 SWAP 0 DO I DUP 2 MOD 0 = IF + ELSE DROP THEN LOOP ;");
    run(interpreter, "10 SUM-EVENS");
    const auto& stack = interpreter.getStack();
    ASSERT_EQ(stack.size(), 1);
    EXPECT_EQ(stack[0], 20.0); // 0+2+4+6+8
}

TEST_F(InterpreterTest, CommentsInsideControlFlow) {
    run(interpreter, "1 IF ( true branch ) 100 ELSE ( false branch ) 200 THEN");
    EXPECT_EQ(interpreter.getStack().back(), 100.0);
    run(interpreter, "DROP");

    run(interpreter, "0 IF ( true branch ) 100 ELSE ( false branch ) 200 THEN");
    EXPECT_EQ(interpreter.getStack().back(), 200.0);
}

TEST_F(InterpreterTest, MemoryStoreAndFetchList) {
    // Store 5 values (10, 20, 30, 40, 50) starting at memory address 200.
    run(interpreter, "10 200 !");
    run(interpreter, "20 201 !");
    run(interpreter, "30 202 !");
    run(interpreter, "40 203 !");
    run(interpreter, "50 204 !");

    // Fetch them back onto the stack in order.
    run(interpreter, "200 @ 201 @ 202 @ 203 @ 204 @");

    const auto& stack = interpreter.getStack();
    ASSERT_EQ(stack.size(), 5);
    EXPECT_EQ(stack[0], 10.0);
    EXPECT_EQ(stack[1], 20.0);
    EXPECT_EQ(stack[2], 30.0);
    EXPECT_EQ(stack[3], 40.0);
    EXPECT_EQ(stack[4], 50.0);
}

TEST_F(InterpreterTest, MemoryListManipulationWithLoop) {
    // Store values (i+1)*10 for i=0..4 at addresses 300+i.
    // This stores 10, 20, 30, 40, 50.
    run(interpreter, "5 0 DO I 1 + 10 * 300 I + ! LOOP");

    // Now, sum them up. 0 is the initial sum on the stack.
    run(interpreter, "0 5 0 DO 300 I + @ + LOOP");

    const auto& stack = interpreter.getStack();
    ASSERT_EQ(stack.size(), 1);
    EXPECT_EQ(stack[0], 150.0); // 10+20+30+40+50
}

TEST_F(InterpreterTest, MemoryListModifyAndVerify) {
    // Store 5, 15, 25 at addresses 50, 51, 52
    run(interpreter, "5 50 ! 15 51 ! 25 52 !");

    // Modify the middle element: store 99 at address 51
    run(interpreter, "99 51 !");

    // Fetch all three values
    run(interpreter, "50 @ 51 @ 52 @");

    const auto& stack = interpreter.getStack();
    ASSERT_EQ(stack.size(), 3);
    EXPECT_EQ(stack[0], 5.0);
    EXPECT_EQ(stack[1], 99.0); // Verifying the modified value
    EXPECT_EQ(stack[2], 25.0);
}

TEST_F(InterpreterTest, ReturnStackOperations) {
    run(interpreter, "10 20 >R"); // data: [10], return: [20]
    EXPECT_EQ(interpreter.getStack().size(), 1);
    EXPECT_EQ(interpreter.getStack().back(), 10.0);

    run(interpreter, "R@"); // data: [10, 20], return: [20]
    ASSERT_EQ(interpreter.getStack().size(), 2);
    EXPECT_EQ(interpreter.getStack()[0], 10.0);
    EXPECT_EQ(interpreter.getStack()[1], 20.0);

    run(interpreter, "R>"); // data: [10, 20, 20], return: []
    ASSERT_EQ(interpreter.getStack().size(), 3);
    EXPECT_EQ(interpreter.getStack()[2], 20.0);

    EXPECT_THROW(run(interpreter, "R>"), std::runtime_error); // Return stack underflow
}

TEST_F(InterpreterTest, BubbleSortListInMemory) {
    // --- SETUP ---
    // Store an unsorted list [50, 20, 90, 10] at memory address 400
    run(interpreter, "50 400 !  20 401 !  90 402 !  10 403 !");

    // --- ALGORITHM DEFINITION ---
    // Define a full bubble sort algorithm
    const std::string bubbleSortCode = R"(
        : COMPARE-AND-SWAP ( addr -- )
            DUP @ >R        ( R: val1 )
            1 + DUP @ >R     ( R: val2 val1 )
            R@ R> < IF      ( if val2 < val1 )
                R@ >R         ( R: val1 val2 val1 )
                SWAP !        ( store val1 at addr+1 )
                R> R> DROP !  ( store val2 at addr )
            ELSE
                R> R> DROP DROP
            THEN ;

        : SORT ( addr len -- )
            1 - 0 DO
                OVER OVER + 1 - SWAP DO
                    I COMPARE-AND-SWAP
                LOOP
            LOOP DROP DROP ;
    )";
    run(interpreter, bubbleSortCode);
    run(interpreter, "400 4 SORT"); // Sort the list at address 400 with length 4

    // --- VERIFICATION ---
    run(interpreter, "400 @ 401 @ 402 @ 403 @"); // Fetch the sorted list
    const auto& stack = interpreter.getStack();
    ASSERT_EQ(stack.size(), 4);
    EXPECT_EQ(stack[0], 10.0);
    EXPECT_EQ(stack[1], 20.0);
    EXPECT_EQ(stack[2], 50.0);
    EXPECT_EQ(stack[3], 90.0);
}
