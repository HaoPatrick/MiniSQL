//
// Created by Haoxiangpeng on 5/18/2017.
//
#include "../src/BTree.h"

#include "catch.hpp"

TEST_CASE("Insert Test") {
    BTree<int> t(3);
    std::string result = "";
    t.insert(std::pair<int,int>(10,0));
    t.insert(std::pair<int,int>(20,0));
    t.insert(std::pair<int,int>(5,0));
    t.insert(std::pair<int,int>(6,0));
    t.insert(std::pair<int,int>(12,0));
    t.insert(std::pair<int,int>(30,0));
    t.insert(std::pair<int,int>(7,0));
    t.insert(std::pair<int,int>(17,0));
    t.traverse(result);
    REQUIRE(result == " 5 6 7 10 12 17 20 30");
}


TEST_CASE("Search Test") {
    BTree<int> t(3);
    std::string result = "";
    t.insert(std::pair<int,int>(10,0));
    t.insert(std::pair<int,int>(20,0));
    t.insert(std::pair<int,int>(5,0));
    t.insert(std::pair<int,int>(6,0));
    t.insert(std::pair<int,int>(12,0));
    t.insert(std::pair<int,int>(30,0));
    t.insert(std::pair<int,int>(7,0));
    t.insert(std::pair<int,int>(17,0));
    int k = 6;
    REQUIRE(t.search(k) != NULL);
    k = 19;
    REQUIRE(t.search(k) == NULL);
}

TEST_CASE("Delete Test") {
    BTree<int> t(3);
    std::string result = "";
    t.insert(std::pair<int,int>(1,0));
    t.insert(std::pair<int,int>(3,0));
    t.insert(std::pair<int,int>(7,0));
    t.insert(std::pair<int,int>(10,0));
    t.insert(std::pair<int,int>(11,0));
    t.insert(std::pair<int,int>(13,0));
    t.insert(std::pair<int,int>(14,0));
    t.insert(std::pair<int,int>(15,0));
    t.insert(std::pair<int,int>(18,0));
    t.insert(std::pair<int,int>(16,0));
    t.insert(std::pair<int,int>(19,0));
    t.insert(std::pair<int,int>(24,0));
    t.insert(std::pair<int,int>(25,0));
    t.insert(std::pair<int,int>(26,0));
    t.insert(std::pair<int,int>(21,0));
    t.insert(std::pair<int,int>(4,0));
    t.insert(std::pair<int,int>(5,0));
    t.insert(std::pair<int,int>(20,0));
    t.insert(std::pair<int,int>(22,0));
    t.insert(std::pair<int,int>(2,0));
    t.insert(std::pair<int,int>(17,0));
    t.insert(std::pair<int,int>(12,0));
    t.insert(std::pair<int,int>(6,0));

    t.remove(6);
    t.traverse(result);
    REQUIRE(result == " 1 2 3 4 5 7 10 11 12 13 14 15 16 17 18 19 20 21 22 24 25 26");

    result = "";
    t.remove(13);
    t.traverse(result);
    REQUIRE(result == " 1 2 3 4 5 7 10 11 12 14 15 16 17 18 19 20 21 22 24 25 26");

}