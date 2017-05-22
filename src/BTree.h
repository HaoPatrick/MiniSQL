//
// Created by Haoxiangpeng on 5/22/2017.
//

#pragma once

#include <iostream>


class BTreeNode {
    int *keys;
    int min_degree; //minimum degree
    BTreeNode **child_pointers; //array of child pointers
    int key_num; //current number of keys
    bool leaf; // Is true when node is leaf, else false
public:
    BTreeNode(int _t, bool _leaf);

    void traverse();

    BTreeNode *search(int k);

    int findKey(int k);

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
    int t; //minimum degree
public:
    BTree(int _t) {
        root = NULL;
        t = _t;
    }

    void traverse() {
        if (root != NULL) root->traverse();
    }

    BTreeNode *search(int k) {
        return (root == NULL) ? NULL : root->search(k);
    }

    void insert(int k);

    void remove(int k);
};