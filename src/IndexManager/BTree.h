//
// Created by Haoxiangpeng on 5/22/2017.
//

#pragma once

#include <iostream>
#include <vector>

template<typename T>
class BTree;

template<typename T>
class BTreeNode {
    std::vector<std::pair<int, T>> v_keys;
    int min_degree; //minimum degree
    std::vector<BTreeNode<T> *> v_child_pointers;
    int key_num; //current number of keys
    bool is_leaf; // Is true when node is leaf, else false

public:
    BTreeNode<T>(int _t, bool _leaf);

    ~BTreeNode<T>() {
    }

    void traverse();

    void traverse(std::string &);


    std::pair<int, T> *search(int k);

    uint32_t findKey(int k);

    void insert_non_full(std::pair<int, T>);

    void split_child(int i, BTreeNode<T> *y);

    void remove(int k);

    void remove_from_leaf(int idx);

    void remove_from_nonleaf(int idx);

    std::pair<int, T> get_prev(int idx);

    std::pair<int, T> get_next(int idx);

    void fill(int idx);

    void borrow_from_prev(int idx);

    void borrow_from_next(int idx);

    void merge(int idx);

    friend class BTree<T>;
};

template<typename T>
class BTree {
    BTreeNode<T> *root;
    int min_degree; //minimum degree
public:
    BTree<T>(int _t) {
        root = NULL;
        min_degree = _t;
    }

    void traverse() {
        if (root != NULL) root->traverse();
    }

    void traverse(std::string &result) {
        if (root != NULL) root->traverse(result);
    }

    std::pair<int, T> *search(int k) {
        if (root) {
            return root->search(k);
        } else {
            return NULL;
        }
    }

    ~BTree<T>() {
        delete (root);
    }

    void insert(std::pair<int, T>);

    void remove(int k);
};

template
class BTree<int>;

template
class BTreeNode<int>;