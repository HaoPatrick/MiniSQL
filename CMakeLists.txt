cmake_minimum_required(VERSION 3.7)
project(db)

set(CMAKE_CXX_STANDARD 14)
#set(BOOST_ROOT "C:\\Program Files\\boost_1_63_0")
#set(BOOSTROOT "C:\\Program Files\\boost_1_63_0")
#find_package(Boost)
#IF (Boost_FOUND)
#    include_directories(${Boost_INCLUDE_DIR})
#endif()

set(FLEX_EXECUTABLE "C:\\Program Files\\flex_bison\\flex")
set(BISON_EXECUTABLE "C:\\Program Files\\flex_bison\\bison")
find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

set(INTER_PATH src/Interpreter)
BISON_TARGET(mc_parser
        ${INTER_PATH}/mc_parser.yy
        ${CMAKE_CURRENT_BINARY_DIR}/mc_parser.tab.cc)

FLEX_TARGET(mc_lexer
        ${INTER_PATH}/mc_lexer.l
        ${CMAKE_CURRENT_BINARY_DIR}/mc_lexer.yy.cc)

ADD_FLEX_BISON_DEPENDENCY(mc_lexer mc_parser)

add_executable(my_wc
        ${INTER_PATH}/main.cpp
        ${INTER_PATH}/mc_driver.cpp
        ${FLEX_mc_lexer_OUTPUTS}
        ${BISON_mc_parser_OUTPUTS}
        )

target_include_directories(my_wc
        PRIVATE
        ${INTER_PATH}
        .
        ${CMAKE_CURRENT_BINARY_DIR}
        )

set_property(TARGET my_wc
        PROPERTY CXX_STANDARD 14)

set(SOURCE_FILES src/main.cpp src/IndexManager/utils.h)
set(TEST_DIR tests)
set(TEST_FILES ${TEST_DIR}/tests_db.cpp)


add_library(lib_tree src/IndexManager/BTree.h src/IndexManager/BTree.cpp)
add_library(lib_catch tests/test_catch.cpp)
add_library(lib_catalog src/CatalogManager/Catalog.cpp src/CatalogManager/Catalog.h)
add_library(lib_file_handler src/BufferManager/FileHandler.cpp src/BufferManager/FileHandler.h)
add_library(lib_buffer src/BufferManager/BufferBlock.cpp src/BufferManager/BufferBlock.h)
add_library(lib_record src/RecordManager/Record.cpp src/RecordManager/Record.h)
add_library(lib_api src/API/API.cpp src/API/API.h)
add_library(lib_api_connector src/API/API_c_connector.cpp src/API/API_c_connector.h)

add_executable(db_test ${TEST_FILES})
add_executable(db ${SOURCE_FILES})
add_executable(db_benchmark ${TEST_DIR}/test_benchmark.cpp)

target_link_libraries(db lib_buffer lib_tree lib_file_handler lib_catalog lib_record)
target_link_libraries(db_test lib_tree lib_catch lib_file_handler lib_record lib_catalog lib_api)
target_link_libraries(db_benchmark lib_tree lib_catch lib_file_handler lib_record lib_catalog lib_api)
#target_link_libraries(db lib_tree)