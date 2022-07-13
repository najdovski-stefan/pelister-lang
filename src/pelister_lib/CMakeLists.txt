add_library(pelister_lib
    lexer.cpp
    parser.cpp
)

target_include_directories(pelister_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
