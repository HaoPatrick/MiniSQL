cmake_minimum_required(VERSION 3.7)
project(db)

set(CMAKE_CXX_STANDARD 11)

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

target_link_libraries(db lib_buffer lib_tree lib_file_handler lib_catalog lib_record)
target_link_libraries(db_test lib_tree lib_catch lib_file_handler lib_record lib_catalog lib_api)
#target_link_libraries(db lib_tree)