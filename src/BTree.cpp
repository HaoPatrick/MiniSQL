//
// Created by Haoxiangpeng on 5/22/2017.
//

#include "BTree.h"

BTreeNode::BTreeNode(int _t, bool _leaf) {
    this->min_degree = _t;
    this->leaf = _leaf;
    this->keys = new int[2 * min_degree - 1];
    this->child_pointers = new BTreeNode *[2 * min_degree];
    this->key_num = 0;
}

int BTreeNode::findKey(int k) {
    int idx = 0;
    while (idx < key_num && keys[idx] < k) {
        idx += 1;
    }
    return idx;
}

void BTreeNode::remove(int k) {
    int idx = findKey(k);
    if (idx < key_num && keys[idx] == k) {
        if (leaf) {
            removeFromLeaf(idx);
        } else {
            removeFromNonLeaf(idx);
        }
    } else {
        if (leaf) {
            std::cout << "The key " << k << " does not exist in the tree\n";
            return;
        }
        bool flag = (idx == key_num);
        if (child_pointers[idx]->key_num < min_degree) {
            fill(idx);
        }
        if (flag && idx > key_num) {
            child_pointers[idx - 1]->remove(k);
        } else {
            child_pointers[idx]->remove(k);
        }
    }
    return;
}

void BTreeNode::removeFromLeaf(int idx) {
    for (int i = idx + 1; i < key_num; i++) {
        keys[i - 1] = keys[i];
    }
    key_num -= 1;
    return;
}

void BTreeNode::removeFromNonLeaf(int idx) {
    int k = keys[idx];
    if (child_pointers[idx]->key_num >= min_degree) {
        int pred = getPred(idx);
        keys[idx] = pred;
        child_pointers[idx]->remove(pred);
    } else if (child_pointers[idx + 1]->key_num >= min_degree) {
        int succ = getSucc(idx);
        keys[idx] = succ;
        child_pointers[idx + 1]->remove(succ);
    } else {
        merge(idx);
        child_pointers[idx]->remove(k);
    }
    return;
}

int BTreeNode::getPred(int idx) {
    BTreeNode *cur = child_pointers[idx];
    while (!cur->leaf) {
        cur = cur->child_pointers[cur->key_num];
    }
    return cur->keys[cur->key_num - 1];
}

int BTreeNode::getSucc(int idx) {
    BTreeNode *cur = child_pointers[idx + 1];
    while (!cur->leaf) {
        cur = cur->child_pointers[0];
    }
    return cur->keys[0];
}

void BTreeNode::fill(int idx) {
    if (idx != 0 && child_pointers[idx - 1]->key_num >= min_degree) {
        borrowFromPrev(idx);
    } else if (idx != key_num && child_pointers[idx + 1]->key_num >= min_degree) {
        borrowFromNext(idx);
    } else {
        if (idx != key_num) {
            merge(idx);
        } else {
            merge(idx - 1);
        }
    }
    return;
}

void BTreeNode::borrowFromPrev(int idx) {
    BTreeNode *child = child_pointers[idx];
    BTreeNode *sibling = child_pointers[idx - 1];
    for (int i = child->key_num - 1; i >= 0; i--) {
        child->keys[i + 1] = child->keys[i];
    }
    if (!child->leaf) {
        for (int i = child->key_num; i >= 0; i--) {
            child->child_pointers[i + 1] = child->child_pointers[i];
        }
    }
    child->keys[0] = keys[idx - 1];
    if (!leaf) {
        child->child_pointers[0] = sibling->child_pointers[sibling->key_num];
    }
    keys[idx - 1] = sibling->keys[sibling->key_num - 1];
    child->key_num += 1;
    sibling->key_num -= 1;
    return;
}

void BTreeNode::borrowFromNext(int idx) {
    BTreeNode *child = child_pointers[idx];
    BTreeNode *sibling = child_pointers[idx + 1];

    child->keys[child->key_num] = keys[idx];
    if (!child->leaf) {
        child->child_pointers[child->key_num + 1] = sibling->child_pointers[0];
    }
    keys[idx] = sibling->keys[0];
    for (int i = 1; i < sibling->key_num; i++) {
        sibling->keys[i - 1] = sibling->keys[i];
    }
    if (!sibling->leaf) {
        for (int i = 1; i <= sibling->key_num; i++) {
            sibling->child_pointers[i - 1] = sibling->child_pointers[i];
        }
    }
    child->key_num += 1;
    sibling->key_num -= 1;
    return;
}

void BTreeNode::merge(int idx) {
    BTreeNode *child = child_pointers[idx];
    BTreeNode *sibling = child_pointers[idx + 1];
    child->keys[min_degree - 1] = keys[idx];
    for (int i = 0; i < sibling->key_num; i++) {
        child->keys[i + min_degree] = sibling->keys[i];
    }
    if (!child->leaf) {
        for (int i = 0; i <= sibling->key_num; i++) {
            child->child_pointers[i + min_degree] = sibling->child_pointers[i];
        }
    }
    for (int i = idx + 1; i < key_num; i++) {
        keys[i - 1] = keys[i];
    }
    for (int i = idx + 2; i <= key_num; i++) {
        child_pointers[i - 1] = child_pointers[i];
    }
    child->key_num += sibling->key_num + 1;
    key_num -= 1;
    delete (sibling);
    return;
}

void BTreeNode::traverse() {
    int i;
    for (i = 0; i < key_num; i++) {
        if (!leaf) {
            child_pointers[i]->traverse();
        }
        std::cout << " " << keys[i];
    }
    // i=n
    if (!leaf) {
        child_pointers[i]->traverse();
    }
}

void BTreeNode::traverse(std::string &result) {
    int i;
    for (i = 0; i < key_num; i++) {
        if (!leaf) {
            child_pointers[i]->traverse(result);
        }
        result += (" " + std::to_string(keys[i]));
    }
    // i=n
    if (!leaf) {
        child_pointers[i]->traverse(result);
    }
}

BTreeNode *BTreeNode::search(int k) {
    int i = 0;
    while (i < key_num && k > keys[i]) {
        i++;
    }
    if (keys[i] == k) {
        return this;
    }
    // Not find here
    if (leaf) {
        return NULL;
    }
    return child_pointers[i]->search(k);
}

void BTreeNode::insertNonFull(int k) {
    int i = key_num - 1;
    if (leaf) {
        while (i >= 0 && keys[i] > k) {
            keys[i + 1] = keys[i];
            i -= 1;
        }
        keys[i + 1] = k;
        key_num += 1;
    } else {
        while (i >= 0 && keys[i] > k) {
            i -= 1;
        }
        if (child_pointers[i + 1]->key_num == 2 * min_degree - 1) {
            splitChild(i + 1, child_pointers[i + 1]);
            if (keys[i + 1] < k) {
                i += 1;
            }
        }
        child_pointers[i + 1]->insertNonFull(k);
    }
}

void BTreeNode::splitChild(int i, BTreeNode *y) {
    BTreeNode *z = new BTreeNode(y->min_degree, y->leaf);
    z->key_num = min_degree - 1;
    for (int j = 0; j < min_degree - 1; j++) {
        z->keys[j] = y->keys[j + min_degree];
    }
    if (!y->leaf) {
        for (int j = 0; j < min_degree; j++) {
            z->child_pointers[j] = y->child_pointers[j + min_degree];
        }
    }
    y->key_num = min_degree - 1;
    for (int j = key_num; j >= i + 1; j--) {
        child_pointers[j + 1] = child_pointers[j];
    }
    child_pointers[i + 1] = z;
    for (int j = key_num - 1; j >= i; j--) {
        keys[j + 1] = keys[j];
    }
    keys[i] = y->keys[min_degree - 1];
    key_num += 1;
}


void BTree::insert(int k) {
    if (root == NULL) {
        root = new BTreeNode(t, true);
        root->keys[0] = k;
        root->key_num = 1;
    } else {
        if (root->key_num == 2 * t - 1) {
            BTreeNode *s = new BTreeNode(t, false);
            s->child_pointers[0] = root;
            s->splitChild(0, root);

            int i = 0;
            if (s->keys[0] < k) {
                i += 1;
            }
            s->child_pointers[i]->insertNonFull(k);
            root = s;
        } else {
            root->insertNonFull(k);
        }
    }
}

void BTree::remove(int k) {
    if (!root) {
        std::cout << "Tree is empty" << std::endl;
        return;
    }
    root->remove(k);
    if (root->key_num == 0) {
        if (root->leaf)
            root = NULL;
        else
            root = root->child_pointers[0];
    }
    return;
}
