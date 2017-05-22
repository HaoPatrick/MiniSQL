cmake_minimum_required(VERSION 3.7)
project(db)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES src/main.cpp src/utils.h)


add_library(db_LIB src/BTree.h src/BTree.cpp)
add_library(db_catch tests/test_catch.cpp)


set(TEST_DIR tests)
set(TEST_FILES  ${TEST_DIR}/tests.cpp)
add_executable(db_test ${TEST_FILES})
add_executable(db ${SOURCE_FILES})
target_link_libraries(db db_LIB)
target_link_libraries(db_test db_LIB db_catch)
#target_link_libraries(db_test testsatch.o )/test_c