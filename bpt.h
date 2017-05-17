//
// Created by Haoxiangpeng on 5/17/2017.
//

#pragma once

#include <utility>
#include <cstdlib>
#include <vector>

namespace trees {
    template<typename Key, typename Value>
    class bpt {
    public:
        struct node_data : std::pair<Key, Value> {
            node_data() = default;

            node_data(const Key &k, const Value &v) {
                this->first = k;
                this->second = v;
            }

            bool operator==(const node_data &other) {
                return this->first = other.first;
            }

            bool operator<=(const node_data &other) {
                return this->first <= other.first;
            }

            bool operator>(const node_data &other) {
                return this->first > other.first;
            }
        };

        struct Node {
            typedef Node *Ptr;
            typedef size_t Weak;
            typedef std::vector<node_data> value_vector;
            typedef std::vector<Weak> child_vector;
            value_vector vals;
            size_t vals_size;
            child_vector childs;
            size_t childs_size;
            size_t id;
            bool is_leaf;

            typename Node::Weak parent;
            typename Node::Weak next;

            Node();

            Node(size_t cap);

            ~Node();

            void insertValue(Key key, Value val);

            void insertChild(Key key, typename Node::Ptr C);
        };

    public:
        bpt() = delete;

        bpt(size_t N);

        ~bpt();

        Value find(Key key) const;

        typename Node::Ptr find_node(Key key) const;

        bool insert(Key key, Value val);

        typename Node::Ptr getNode(const typename Node::Weak &w);

        typename Node::Ptr getNode(const typename Node::Weak &w) const;

    protected:
        typename Node::Ptr make_node();

        bool inner_find(Key key, typename Node::Ptr cur_node, typename Node::Ptr &out_ptr, Value &out_res) const;

        bool is_full(const typename Node::Ptr node) const;

        void split_node(typename Node::Ptr node);

    private:
        size_t n;
        std::vector<Node> *cache;
        size_t cache_pos;
        typename Node::Ptr m_root;
    };
}

#include "bpt.cpp"

