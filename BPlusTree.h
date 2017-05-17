//
// Created by Haoxiangpeng on 5/17/2017.
//

#pragma once

#include <string>
#include <vector>

//template<typename Key, typename Value>
class Node;

//template<class Node>
class Bucket {
public:
    Bucket(int);

    bool is_root;

    bool is_leaf;
    bool is_inner;

    bool insert_to_cur_bucket(Node);

    void split_bucket();

    void print_bucket();

private:
    int degree;
    Bucket *parent;
    std::vector<Node> nodes;
};

//template<typename Key, typename Value>
class Node {
public:
    std::pair<int, std::string> nodes;
    std::vector<Bucket *> pointers;
};

