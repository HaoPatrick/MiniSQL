//
// Created by Haoxiangpeng on 5/17/2017.
//
#include "bpt.h"

namespace trees {
    template<class Key, class Value>
    bpt<Key, Value>::Node::Node() {
        is_leaf = false;
        vals_size = 0;
        childs_size = 0;
        id = 0;
        parent = 0;
        next = 0;
    };

    template<class Key, class Value>
    bpt<Key, Value>::Node::Node(size_t cap) {
        is_leaf = false;
        this->childs.resize(cap + 1);
        this->vals.resize(cap + 1);
        vals_size = 0;
        childs_size = 0;
        id = 0;
        parent = 0;
        next = 0;
    }

    template<class Key, class Value>
    bpt<Key, Value>::Node::~Node() {
        this->childs.clear();
        this->vals.clear();
    }

    template<class Key, class Value>
    void bpt<Key, Value>::Node::insertValue(Key key, Value val) {
        auto kv = node_data(key, val);
        if (vals_size == 0) {
            vals_size += 1;
            vals[0] = kv;
            return;
        } else {
            if (vals.data()->first() > key) {
                vals_size+=1;

            }
        }
    }

    template<class Key, class Value>
    void bpt<Key, Value>::Node::insertChild(Key key, typename Node::Ptr C) {

    }
}