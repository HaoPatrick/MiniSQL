//
// Created by Haoxiangpeng on 5/18/2017.
//
#include "../src/Indexmanager/BTree.h"
#include "../src/BufferManager/FileHandler.h"
#include "../src/API/API.h"
#include <cstring>
#include <functional>
#include "catch.hpp"

TEST_CASE("Test API", "[API]") {
    API instance;
    std::string table_name = "db_api";
    std::vector<unsigned int> counts;
    counts.push_back(2);
    counts.push_back(1);
    counts.push_back(1);
    std::vector<std::string> attr_name;
    attr_name.push_back("int0");
    attr_name.push_back("int1");
    attr_name.push_back("float0");
    attr_name.push_back("char0");

    instance.create_table(table_name, counts, attr_name);
    Catalog saved_catalog = instance.load_table("db_api");

    CHECK(saved_catalog.attr_names[0] == "int0");
    CHECK(saved_catalog.attr_names[3] == "char0");
    CHECK(saved_catalog.table_name.to_string() == "db_api");
    CHECK(saved_catalog.char_count == 1);
}

TEST_CASE("Write and Load catalog", "[Catalog]") {
    DBHeader test_header;
    strncpy(test_header.db_name, "HLH DB",
            sizeof(test_header.db_name) - 1);
    test_header.type = catalog;
    test_header.count = 1;
    test_header.int_count = 2;
    test_header.float_count = 1;
    test_header.char_count = 1;
    test_header.check_value = 42;
    FileHandler aa("catalog.hlh", test_header);

    Catalog catalog(2, 1, 1);
    catalog.table_name = "test table";
    catalog.attr_names[0] = "int 0";
    catalog.attr_names[1] = "int 1";
    catalog.attr_names[2] = "float 0";
    catalog.attr_names[3] = "char 0";

    aa.write_catalog(catalog);

    Catalog result_catalog = aa.load_catalog();
    CHECK(result_catalog.table_name.to_string() == "test table");
    CHECK(result_catalog.attr_names[0].to_string() == "int 0");
    CHECK(result_catalog.int_count == 2);
}

TEST_CASE("Catalog and Record Test", "[Catalog]") {
    DBHeader test_header;
    strncpy(test_header.db_name, "HLH DB", sizeof(test_header.db_name) - 1);
    test_header.type = table;
    test_header.count = 30;
    test_header.int_count = 2;
    test_header.float_count = 1;
    test_header.char_count = 1;
    test_header.check_value = 42;
    FileHandler aa("new.hlh", test_header);

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


    aa.write_sample_data(record, 30);

    Record result_record(catalog);
    std::string result_string = aa.read_data(3, record);
    CHECK(result_string == "3 24 3.010000 Hello hlh! ");
    result_string = aa.read_data(23, record);
    CHECK(result_string == "23 24 23.010000 Hello hlh! ");

    bool search_result;
    SECTION("Linear int search") {
        search_result = aa.linear_search(result_record, 0, 10);
        CHECK(result_record.int_v[0] == 10);
        CHECK(search_result);

        search_result = aa.linear_search(result_record, 0, 23);
        CHECK(result_record.int_v[0] == 23);
        CHECK(search_result);
    }

    SECTION("Linear float search") {
        search_result = aa.linear_search(result_record, 0, (float) 10.01);
        CHECK(result_record.float_v[0] == (float) 10.01);
        CHECK(search_result);

        search_result = aa.linear_search(result_record, 0, (float) 23.01);
        CHECK(result_record.float_v[0] == (float) 23.01);
        CHECK(search_result);
    }
    SECTION("Linear string search") {
        search_result = aa.linear_search(result_record, 0, "Hello hlh!");
        CHECK(result_record.int_v[0] == 0);
        CHECK(search_result);
    }

    SECTION("Interval search int") {
        std::vector<Record> result = aa.interval_search(0, 3, result_record, std::less<int>());
        CHECK(result.size() == 3);

        result = aa.interval_search(0, 3, result_record, std::less_equal<int>());
        CHECK(result.size() == 4);
        result = aa.interval_search(0, 3, result_record, std::greater<int>());
        CHECK(result.size() == 26);
        result = aa.interval_search(0, 3, result_record, std::greater_equal<int>());
        CHECK(result.size() == 27);
    }
    SECTION("Interval search float") {
        std::vector<Record> result = aa.interval_search(0, (float) 3.01, result_record, std::less<float>());
        CHECK(result.size() == 3);

        result = aa.interval_search(0, (float) 3.01, result_record, std::less_equal<float>());
        CHECK(result.size() == 4);
        result = aa.interval_search(0, (float) 3.01, result_record, std::greater<float>());
        CHECK(result.size() == 26);
        result = aa.interval_search(0, (float) 3.01, result_record, std::greater_equal<float>());
        CHECK(result.size() == 27);
    }

    record.int_v[0] = 1111;
    record.float_v[0] = 2.718;
    record.char_v[0] = FixString("Great WBX");
    aa.insert_record(record);
    result_string = aa.read_data(31, record);
    CHECK(result_string == "1111 24 2.718000 Great WBX ");
}

TEST_CASE("Buffer Test", "[Buffer]") {
    SampleRecord test_data;
    DBHeader test_header;
    strncpy(test_header.db_name, "HLH DB", sizeof(test_header.db_name) - 1);
    test_header.type = table;
    test_header.count = 30;
    test_header.int_count = 2;
    test_header.char_count = 2;
    test_header.float_count = 0;
//    memset(test_header.ava_slot, 0, sizeof(test_header.ava_slot));

    strncpy(test_data.title, "Great wbx", sizeof(test_data.title) - 1);
    strncpy(test_data.comment, "Hey wbx", sizeof(test_data.comment) - 1);

    std::string file_path("db_test.hlh");
    FileHandler aa(file_path);

    aa.write_sample_data(test_header, test_data);
    std::string result_string("");

    SECTION("Read & Write Test") {
        result_string = aa.read_data(29, test_data);
        CHECK(result_string == "Great wbx 29 87");
    }

    SECTION("Build tree Test") {
        BTree<int> tree = aa.build_tree();
        std::string result("");
        tree.traverse(result);
        CHECK(result == " 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29");
    }

    SECTION("Write & Load tree Test") {
        std::string result("");
        BTree<int> tree = aa.build_tree();
        aa.write_tree(tree);
        BTree<int> tree2(3);
        aa.load_tree(tree2);
        tree2.traverse(result);
        CHECK(result == " 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29");
    }

    SECTION("Append data Test") {
        test_data.index = 42;
        test_data.views = 77;
        aa.append_data(test_data, test_header);
        result_string = aa.read_data(30, test_data);
        CHECK(result_string == "Great wbx 77 42");
    }

}


TEST_CASE("B Tree Test", "[B Tree]") {
    BTree<int> t(3);
    std::string result = "";
    t.insert(std::pair<int, int>(1, 0));
    t.insert(std::pair<int, int>(3, 0));
    t.insert(std::pair<int, int>(7, 0));
    t.insert(std::pair<int, int>(10, 0));
    t.insert(std::pair<int, int>(11, 0));
    t.insert(std::pair<int, int>(13, 0));
    t.insert(std::pair<int, int>(14, 0));
    t.insert(std::pair<int, int>(15, 0));
    t.insert(std::pair<int, int>(18, 0));
    t.insert(std::pair<int, int>(16, 0));
    t.insert(std::pair<int, int>(19, 0));
    t.insert(std::pair<int, int>(24, 0));
    t.insert(std::pair<int, int>(25, 0));
    t.insert(std::pair<int, int>(26, 0));
    t.insert(std::pair<int, int>(21, 0));
    t.insert(std::pair<int, int>(4, 0));
    t.insert(std::pair<int, int>(5, 0));
    t.insert(std::pair<int, int>(20, 0));
    t.insert(std::pair<int, int>(22, 0));
    t.insert(std::pair<int, int>(2, 0));
    t.insert(std::pair<int, int>(17, 0));
    t.insert(std::pair<int, int>(12, 0));
    t.insert(std::pair<int, int>(6, 10));

    SECTION("Traverse Test") {
        t.traverse(result);
        CHECK(result == " 1 2 3 4 5 6 7 10 11 12 13 14 15 16 17 18 19 20 21 22 24 25 26");

        std::vector<std::pair<int, int>> result_v;
        t.traverse(result_v);
        CHECK(result_v[0].first == 1);
        CHECK(result_v[3].first == 4);
    }
    SECTION("Search Test") {
        int k = 6;
        CHECK(t.search(k)->second == 10);
        k = 190;
        CHECK(t.search(k) == NULL);
    }
    SECTION("Delete Test") {
        t.remove(6);
        t.traverse(result);
        CHECK(result == " 1 2 3 4 5 7 10 11 12 13 14 15 16 17 18 19 20 21 22 24 25 26");

        result = "";
        t.remove(13);
        t.traverse(result);
        CHECK(result == " 1 2 3 4 5 7 10 11 12 14 15 16 17 18 19 20 21 22 24 25 26");
    }
}
