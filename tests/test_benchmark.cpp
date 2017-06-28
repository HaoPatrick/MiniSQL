//
// Created by Haoxiangpeng on 6/14/2017.
//
#include "../src/Indexmanager/BTree.h"
#include "../src/BufferManager/FileHandler.h"
#include "../src/API/API.h"
#include <cstring>
#include <iostream>
#include <chrono>
#include "catch.hpp"

using namespace std::chrono;

TEST_CASE("Catalog and Record Test", "[Catalog]") {
    DBHeader test_header;
    strncpy(test_header.db_name, "HLH DB", sizeof(test_header.db_name) - 1);
    test_header.type = table_type;
    test_header.count = 30;
//    test_header.int_count = 2;
//    test_header.float_count = 1;
//    test_header.char_count = 1;
    test_header.check_value = 42;
    FileHandler aa("benchmark.hlh", test_header);

    Catalog catalog(2, 1, 1);
    catalog.table_name = "test table";
    catalog.attr_names[0] = "int 0";
    catalog.attr_names[1] = "int 1";
    catalog.attr_names[2] = "float 0";
    catalog.attr_names[3] = "char 0";

    Record record(catalog);
    record.int_v[0] = 42;
    record.int_v[1] = 24;
    record.float_v[0] = 3.1415;
//    strncpy(record.char_v[0], "hello hlh!", sizeof(record.char_v[0]) - 1);
    record.char_v[0] = FixString("Hello hlh!");


    aa.write_sample_data(record, 30000);

    Record result_record(catalog);
    std::string result_string = aa.read_data(3, record);
    REQUIRE(result_string == "3 24 3.010000 Hello hlh! ");
    aa.read_data(150, record);
    REQUIRE(record.int_v[0] == 150);

    SECTION("Int Bench") {
        std::chrono::time_point<std::chrono::system_clock> start, end;
        start = std::chrono::system_clock::now();

        bool search_result = aa.linear_search(result_record, 0, 29999);

        end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;
        std::cout << "Time elapsed(int) : " << elapsed_seconds.count() << std::endl;

        REQUIRE(search_result);
        REQUIRE(result_record.int_v[0] == 29999);
    }
    SECTION("Float Bench") {
        std::chrono::time_point<std::chrono::system_clock> start, end;
        start = std::chrono::system_clock::now();

        bool search_result = aa.linear_search(result_record, 0, (float) 29999.01);

        end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;
        std::cout << "Time elapsed(float) : " << elapsed_seconds.count() << std::endl;

        REQUIRE(search_result);
        REQUIRE(result_record.int_v[0] == (float) 29999);
    }
}