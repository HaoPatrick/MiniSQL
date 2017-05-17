//
// Created by Haoxiangpeng on 5/17/2017.
//

#include <iostream>
#include "BPlusTree.h"

Bucket::Bucket(int degree) {
    this->degree = degree;
    this->parent = nullptr;
    type = root;
}

//template <class Node>
bool Bucket::insert_to_cur_bucket(Node node2insert) {
    uint32_t pos2insert = 0;
    while (pos2insert < this->nodes.size()) {
        if (node2insert.value.first > this->nodes.at(pos2insert).value.first) {
            break;
        } else if (node2insert.value.first == this->nodes.at(pos2insert).value.first) {
            return false;
        }
        pos2insert += 1;
    }
    this->nodes.insert(this->nodes.begin() + pos2insert, node2insert);
    if (this->type == leaf && this->nodes.size() > degree - 1) {
//        split_bucket();
    } else if (this->type == inner && this->nodes.size() > degree) {
//        split_bucket();
    }
    return true;
}


void Bucket::print_bucket() {
    std::cout << this->string_bucket() << std::endl;
}

std::string Bucket::string_bucket() {
    std::string output_str;
    for (auto node:nodes) {
        output_str += (std::to_string(node.value.first) + "-" + node.value.second + " ");
    }
    return output_str;
}


Bucket Bucket::find_bucket(Node node2find) {
    size_t pos_found = 0;
    if (this->type == leaf) {
        return *this;
    }
    while (pos_found < nodes.size()) {
        if (nodes.at(pos_found).value.first == node2find.value.first) {
            break;
        }
        pos_found += 1;
    }
    if (nodes.at(pos_found).pointers) {
        Bucket new_bucket = *nodes.at(pos_found).pointers;
        if (new_bucket.type != leaf || new_bucket.type != root) {
            return new_bucket.find_bucket(node2find);
        } else {
            return new_bucket;
        }
    } else {
        return *this;
    }
}
