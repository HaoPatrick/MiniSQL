//
// Created by Haoxiangpeng on 5/22/2017.
//

#include "BTree.h"

BTreeNode::BTreeNode(int _t, bool _leaf) {
    this->min_degree = _t;
    this->is_leaf = _leaf;
    this->keys = new int[2 * min_degree - 1];
    this->v_keys.resize((unsigned int) (2 * min_degree - 1));
    this->child_pointers = new BTreeNode *[2 * min_degree];
    this->v_child_pointers.resize((unsigned int) (2 * min_degree));
    this->key_num = 0;
}

uint32_t BTreeNode::findKey(int k) {
    uint32_t idx = 0;
    while (idx < key_num && v_keys[idx] < k) {
        idx += 1;
    }
    return idx;
}

void BTreeNode::remove(int k) {
    uint32_t idx = findKey(k);
    if (idx < key_num && v_keys[idx] == k) {
        if (is_leaf) {
            removeFromLeaf(idx);
        } else {
            removeFromNonLeaf(idx);
        }
    } else {
        if (is_leaf) {
            std::cout << "The key " << k << " does not exist in the tree\n";
            return;
        }
        bool flag = (idx == key_num);
        if (v_child_pointers[idx]->key_num < min_degree) {
            fill(idx);
        }
        if (flag && idx > key_num) {
            v_child_pointers[idx - 1]->remove(k);
        } else {
            v_child_pointers[idx]->remove(k);
        }
    }
    return;
}

void BTreeNode::removeFromLeaf(int idx) {
    for (int i = idx + 1; i < key_num; i++) {
        v_keys[i - 1] = v_keys[i];
    }
    key_num -= 1;
    return;
}

void BTreeNode::removeFromNonLeaf(int idx) {
    int k = v_keys[idx];
    if (v_child_pointers[idx]->key_num >= min_degree) {
        int pred = getPred(idx);
        v_keys[idx] = pred;
        v_child_pointers[idx]->remove(pred);
    } else if (v_child_pointers[idx + 1]->key_num >= min_degree) {
        int succ = getSucc(idx);
        v_keys[idx] = succ;
        v_child_pointers[idx + 1]->remove(succ);
    } else {
        merge(idx);
        v_child_pointers[idx]->remove(k);
    }
    return;
}

int BTreeNode::getPred(int idx) {
    BTreeNode *cur = v_child_pointers[idx];
    while (!cur->is_leaf) {
        cur = cur->v_child_pointers[cur->key_num];
    }
    return cur->v_keys[cur->key_num - 1];
}

int BTreeNode::getSucc(int idx) {
    BTreeNode *cur = v_child_pointers[idx + 1];
    while (!cur->is_leaf) {
        cur = cur->v_child_pointers[0];
    }
    return cur->v_keys[0];
}

void BTreeNode::fill(int idx) {
    if (idx != 0 && v_child_pointers[idx - 1]->key_num >= min_degree) {
        borrowFromPrev(idx);
    } else if (idx != key_num && v_child_pointers[idx + 1]->key_num >= min_degree) {
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
    BTreeNode *child = v_child_pointers[idx];
    BTreeNode *sibling = v_child_pointers[idx - 1];
    for (int i = child->key_num - 1; i >= 0; i--) {
        child->v_keys[i + 1] = child->v_keys[i];
    }
    if (!child->is_leaf) {
        for (int i = child->key_num; i >= 0; i--) {
            child->v_child_pointers[i + 1] = child->v_child_pointers[i];
        }
    }
    child->v_keys[0] = v_keys[idx - 1];
    if (!is_leaf) {
        child->v_child_pointers[0] = sibling->v_child_pointers[sibling->key_num];
    }
    v_keys[idx - 1] = sibling->v_keys[sibling->key_num - 1];
    child->key_num += 1;
    sibling->key_num -= 1;
    return;
}

void BTreeNode::borrowFromNext(int idx) {
    BTreeNode *child = v_child_pointers[idx];
    BTreeNode *sibling = v_child_pointers[idx + 1];

    child->v_keys[child->key_num] = v_keys[idx];
    if (!child->is_leaf) {
        child->v_child_pointers[child->key_num + 1] = sibling->v_child_pointers[0];
    }
    v_keys[idx] = sibling->v_keys[0];
    for (int i = 1; i < sibling->key_num; i++) {
        sibling->v_keys[i - 1] = sibling->v_keys[i];
    }
    if (!sibling->is_leaf) {
        for (int i = 1; i <= sibling->key_num; i++) {
            sibling->v_child_pointers[i - 1] = sibling->v_child_pointers[i];
        }
    }
    child->key_num += 1;
    sibling->key_num -= 1;
    return;
}

void BTreeNode::merge(int idx) {
    BTreeNode *child = v_child_pointers[idx];
    BTreeNode *sibling = v_child_pointers[idx + 1];
    child->v_keys[min_degree - 1] = v_keys[idx];
    for (int i = 0; i < sibling->key_num; i++) {
        child->v_keys[i + min_degree] = sibling->v_keys[i];
    }
    if (!child->is_leaf) {
        for (int i = 0; i <= sibling->key_num; i++) {
            child->v_child_pointers[i + min_degree] = sibling->v_child_pointers[i];
        }
    }
    for (int i = idx + 1; i < key_num; i++) {
        v_keys[i - 1] = v_keys[i];
    }
    for (int i = idx + 2; i <= key_num; i++) {
        v_child_pointers[i - 1] = v_child_pointers[i];
    }
    child->key_num += sibling->key_num + 1;
    key_num -= 1;
    delete (sibling);
    return;
}

void BTreeNode::traverse() {
    int i;
    for (i = 0; i < key_num; i++) {
        if (!is_leaf) {
            v_child_pointers[i]->traverse();
        }
        std::cout << " " << v_keys[i];
    }
    // i=n
    if (!is_leaf) {
        v_child_pointers[i]->traverse();
    }
}

void BTreeNode::traverse(std::string &result) {
    int i;
    for (i = 0; i < key_num; i++) {
        if (!is_leaf) {
            v_child_pointers[i]->traverse(result);
        }
        result += (" " + std::to_string(v_keys[i]));
    }
    // i=n
    if (!is_leaf) {
        v_child_pointers[i]->traverse(result);
    }
}

BTreeNode *BTreeNode::search(int k) {
    int i = 0;
    while (i < key_num && k > v_keys[i]) {
        i++;
    }
    if (v_keys[i] == k) {
        return this;
    }
    // Not find here
    if (is_leaf) {
        return NULL;
    }
    return v_child_pointers[i]->search(k);
}

void BTreeNode::insertNonFull(int k) {
    int i = key_num - 1;
    if (is_leaf) {
        while (i >= 0 && v_keys[i] > k) {
            v_keys[i + 1] = v_keys[i];
            i -= 1;
        }
        v_keys[i + 1] = k;
        key_num += 1;
    } else {
        while (i >= 0 && v_keys[i] > k) {
            i -= 1;
        }
        if (v_child_pointers[i + 1]->key_num == 2 * min_degree - 1) {
            splitChild(i + 1, v_child_pointers[i + 1]);
            if (v_keys[i + 1] < k) {
                i += 1;
            }
        }
        v_child_pointers[i + 1]->insertNonFull(k);
    }
}

void BTreeNode::splitChild(int i, BTreeNode *y) {
    BTreeNode *z = new BTreeNode(y->min_degree, y->is_leaf);
    z->key_num = min_degree - 1;
    for (int j = 0; j < min_degree - 1; j++) {
        z->v_keys[j] = y->v_keys[j + min_degree];
    }
    if (!y->is_leaf) {
        for (int j = 0; j < min_degree; j++) {
            z->v_child_pointers[j] = y->v_child_pointers[j + min_degree];
        }
    }
    y->key_num = min_degree - 1;
    for (int j = key_num; j >= i + 1; j--) {
        v_child_pointers[j + 1] = v_child_pointers[j];
    }
    v_child_pointers[i + 1] = z;
    for (int j = key_num - 1; j >= i; j--) {
        v_keys[j + 1] = v_keys[j];
    }
    v_keys[i] = y->v_keys[min_degree - 1];
    key_num += 1;
}


void BTree::insert(int k) {
    if (root == NULL) {
        root = new BTreeNode(min_degree, true);
        root->v_keys[0] = k;
        root->key_num = 1;
    } else {
        if (root->key_num == 2 * min_degree - 1) {
            BTreeNode *s = new BTreeNode(min_degree, false);
            s->v_child_pointers[0] = root;
            s->splitChild(0, root);

            int i = 0;
            if (s->v_keys[0] < k) {
                i += 1;
            }
            s->v_child_pointers[i]->insertNonFull(k);
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
        if (root->is_leaf)
            root = NULL;
        else
            root = root->v_child_pointers[0];
    }
    return;
}
