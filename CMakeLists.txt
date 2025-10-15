cmake_minimum_required(VERSION 3.10)
project(Laba_2)

set(CMAKE_CXX_STANDARD 17)

include_directories(Laba_2/include)

add_executable(main Laba_2/main.cpp Laba_2/src/octal.cpp)

# Tests
enable_testing()
find_package(GTest REQUIRED)
add_executable(tests Laba_2/tests/test.cpp Laba_2/src/octal.cpp)
target_link_libraries(tests GTest::gtest GTest::gtest_main)
add_test(NAME OctalTests COMMAND tests)