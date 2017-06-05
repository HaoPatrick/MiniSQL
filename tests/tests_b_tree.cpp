//
// Created by Haoxiangpeng on 5/18/2017.
//
#include "../src/Indexmanager/BTree.h"
#include "../src/BufferManager/FileHandler.h"
#include <cstring>
#include "catch.hpp"

TEST_CASE("Buffer Test", "[Buffer]") {
    SampleRecord test_data;
    DBHeader test_header;
    strncpy(test_header.db_name, "HLH DB", sizeof(test_header.db_name) - 1);
    test_header.type = table;
    test_header.count = 30;
    test_header.item_size = sizeof(test_data);
    test_header.ultimate_value = 42;
    memset(test_header.ava_slot, 0, sizeof(test_header.ava_slot));

    strncpy(test_data.title, "Great wbx", sizeof(test_data.title) - 1);
    strncpy(test_data.comment, "Hey wbx", sizeof(test_data.comment) - 1);


    FileHandler aa((char *) FILE_PATH);

    aa.write_sample_data(test_header, test_data);
    std::string result_string("");

    SECTION("Read & Write Test") {
        result_string = aa.read_data(29, test_data);
        REQUIRE(result_string == "Great wbx 29 87");
    }

    SECTION("Build tree Test") {
        BTree<int> tree = aa.build_tree();
        std::string result("");
        tree.traverse(result);
        REQUIRE(result == " 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29");
    }

    SECTION("Write & Load tree Test") {
        std::string result("");
        BTree<int> tree = aa.build_tree();
        aa.write_tree(tree);
        BTree<int> tree2(3);
        aa.load_tree(tree2);
        tree2.traverse(result);
        REQUIRE(result == " 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29");
    }

    SECTION("Append data Test") {
        test_data.index = 42;
        test_data.views = 77;
        aa.append_data(test_data, test_header);
        result_string = aa.read_data(30, test_data);
        REQUIRE(result_string == "Great wbx 77 42");
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
        REQUIRE(result == " 1 2 3 4 5 6 7 10 11 12 13 14 15 16 17 18 19 20 21 22 24 25 26");

        std::vector<std::pair<int, int>> result_v;
        t.traverse(result_v);
        REQUIRE(result_v[0].first == 1);
        REQUIRE(result_v[3].first == 4);
    }
    SECTION("Search Test") {
        int k = 6;
        REQUIRE(t.search(k)->second == 10);
        k = 190;
        REQUIRE(t.search(k) == NULL);
    }
    SECTION("Delete Test") {
        t.remove(6);
        t.traverse(result);
        REQUIRE(result == " 1 2 3 4 5 7 10 11 12 13 14 15 16 17 18 19 20 21 22 24 25 26");

        result = "";
        t.remove(13);
        t.traverse(result);
        REQUIRE(result == " 1 2 3 4 5 7 10 11 12 14 15 16 17 18 19 20 21 22 24 25 26");
    }
}
