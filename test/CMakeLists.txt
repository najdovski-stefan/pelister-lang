include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.13.0.zip
)
FetchContent_MakeAvailable(googletest)

add_executable(run_tests
    lexer_test.cpp
    parser_test.cpp
    interpreter_test.cpp
)

target_link_libraries(run_tests
    PRIVATE
        pelister_lib
        gtest_main
)

include(GoogleTest)
gtest_discover_tests(run_tests)
