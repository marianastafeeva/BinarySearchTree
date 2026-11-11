cmake_minimum_required(VERSION 3.16)
project(BinarySearchTree LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/W4 /permissive-)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

include_directories(${CMAKE_SOURCE_DIR}/include)

find_package(GTest REQUIRED)

add_executable(bst
    src/main.cpp
    src/ui.cpp
)

add_executable(bst_tests
    src/tests.cpp
)

target_link_libraries(bst_tests
    PRIVATE
    GTest::gtest
    GTest::gtest_main
)

add_executable(bst_bench
    src/bench.cpp
)