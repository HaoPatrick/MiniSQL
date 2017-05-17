//
// Created by Haoxiangpeng on 5/17/2017.
//

#pragma once

#include <string>
#include <vector>

//template<typename Key, typename Value>
class Node;

enum Type {
    root,
    leaf,
    inner
};

//template<class Node>
class Bucket {
public:
    Bucket(int);

    Type type;

    bool insert_to_cur_bucket(Node);

    Bucket find_bucket(Node);

    void split_bucket();

    void print_bucket();

    std::string string_bucket();

    int degree;
    Bucket *parent;
    std::vector<Node> nodes;
};

//template<typename Key, typename Value>
class Node {
public:
    std::pair<int, std::string> value;
    Bucket *pointers;
};

