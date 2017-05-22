//
// Created by Haoxiangpeng on 5/18/2017.
//
#include "../src/BTree.h"

#include "catch.hpp"

TEST_CASE("Insert Test") {
    BTree t(3);
    std::string result = "";
    t.insert(10);
    t.insert(20);
    t.insert(5);
    t.insert(6);
    t.insert(12);
    t.insert(30);
    t.insert(7);
    t.insert(17);
    t.traverse(result);
    REQUIRE(result == " 5 6 7 10 12 17 20 30");
}


TEST_CASE("Search Test") {
    BTree t(3);
    std::string result = "";
    t.insert(10);
    t.insert(20);
    t.insert(5);
    t.insert(6);
    t.insert(12);
    t.insert(30);
    t.insert(7);
    t.insert(17);
    int k = 6;
    REQUIRE(t.search(k) != NULL);
    k = 19;
    REQUIRE(t.search(k) == NULL);
}

TEST_CASE("Delete Test") {
    BTree t(3);
    std::string result = "";
    t.insert(1);
    t.insert(3);
    t.insert(7);
    t.insert(10);
    t.insert(11);
    t.insert(13);
    t.insert(14);
    t.insert(15);
    t.insert(18);
    t.insert(16);
    t.insert(19);
    t.insert(24);
    t.insert(25);
    t.insert(26);
    t.insert(21);
    t.insert(4);
    t.insert(5);
    t.insert(20);
    t.insert(22);
    t.insert(2);
    t.insert(17);
    t.insert(12);
    t.insert(6);

    t.remove(6);
    t.traverse(result);
    REQUIRE(result == " 1 2 3 4 5 7 10 11 12 13 14 15 16 17 18 19 20 21 22 24 25 26");

    result = "";
    t.remove(13);
    t.traverse(result);
    REQUIRE(result == " 1 2 3 4 5 7 10 11 12 14 15 16 17 18 19 20 21 22 24 25 26");

}