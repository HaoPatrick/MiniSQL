//
// Created by Haoxiangpeng on 5/22/2017.
//

#pragma once

#include <iostream>
#include <vector>


class BTreeNode {
    std::vector<int> v_keys;
    int min_degree; //minimum degree
    std::vector<BTreeNode *> v_child_pointers;
    int key_num; //current number of keys
    bool is_leaf; // Is true when node is leaf, else false
    ~BTreeNode() {
    }

public:
    BTreeNode(int _t, bool _leaf);

    void traverse();

    void traverse(std::string &);


    BTreeNode *search(int k);

    uint32_t findKey(int k);

    void insertNonFull(int k);

    void splitChild(int i, BTreeNode *y);

    void remove(int k);

    void removeFromLeaf(int idx);

    void removeFromNonLeaf(int idx);

    int getPred(int idx);

    int getSucc(int idx);

    void fill(int idx);

    void borrowFromPrev(int idx);

    void borrowFromNext(int idx);

    void merge(int idx);

    friend class BTree;
};

class BTree {
    BTreeNode *root;
    int min_degree; //minimum degree
public:
    BTree(int _t) {
        root = NULL;
        min_degree = _t;
    }

    void traverse() {
        if (root != NULL) root->traverse();
    }

    void traverse(std::string &result) {
        if (root != NULL) root->traverse(result);
    }

    BTreeNode *search(int k) {
        return (root == NULL) ? NULL : root->search(k);
    }
    ~BTree(){
        delete(root);
    }

    void insert(int k);

    void remove(int k);
};