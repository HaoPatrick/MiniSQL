//
// Created by Haoxiangpeng on 5/18/2017.
//
#include "../src/BPlusTree.h"

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include "catch.hpp"

TEST_CASE("Init Test") {
    Bucket bpt(4);
    REQUIRE(bpt.degree == 4);
}

TEST_CASE("InsertTest") {
    Bucket bpt(4);
    Node test_node;
    test_node.pointers = nullptr;
    bpt.type = root;
    test_node.value = std::make_pair(3, "hello");
    bpt.insert_to_cur_bucket(test_node);
    test_node.value = std::make_pair(4, "hello");
    bpt.insert_to_cur_bucket(test_node);
    CHECK(bpt.nodes.size() == 2);
    CHECK(bpt.string_bucket() == "4-hello 3-hello ");
}