add_executable(pelilang main.cpp)

target_link_libraries(pelilang PRIVATE pelister_lib)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
