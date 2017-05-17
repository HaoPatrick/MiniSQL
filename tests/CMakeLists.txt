cmake_minimum_required(VERSION 3.7)
project(db_test)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES tests.cpp catch.hpp)
add_executable(db ${SOURCE_FILES})