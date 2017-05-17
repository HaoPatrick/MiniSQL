//
// Created by Haoxiangpeng on 5/17/2017.
//

#include <iostream>
#include "BPlusTree.h"

Bucket::Bucket(int degree) {
    this->degree = degree;
    this->parent = nullptr;
    this->is_root = false;
    this->is_leaf = false;
    this->is_inner = true;
}

//template <class Node>
bool Bucket::insert_to_cur_bucket(Node node2insert) {
    uint32_t pos2insert = 0;
    while (pos2insert < this->nodes.size()) {
        if (node2insert.nodes.first > this->nodes.at(pos2insert).nodes.first) {
            break;
        } else if (node2insert.nodes.first == this->nodes.at(pos2insert).nodes.first) {
            return false;
        }
        pos2insert += 1;
    }
    this->nodes.insert(this->nodes.begin() + pos2insert, node2insert);
    if (this->is_leaf && this->nodes.size() > degree - 1) {
//        split_bucket();
    } else if (this->is_inner && this->nodes.size() > degree) {
//        split_bucket();
    }
    return true;
}

void Bucket::print_bucket() {
    for (auto node:nodes) {
        std::cout << node.nodes.first << "-" << node.nodes.second << " ";
    }
    std::cout << std::endl;
}