#include "BPlusTree.h"


int main(int argc, char *argv[]) {
    Bucket bpt(4);
    Node test_node;
    test_node.nodes = std::make_pair(3, "hello");
    bpt.insert_to_cur_bucket(test_node);
    test_node.nodes = std::make_pair(4, "hello");
    bpt.insert_to_cur_bucket(test_node);
    test_node.nodes = std::make_pair(5, "hello");
    bpt.insert_to_cur_bucket(test_node);
    bpt.print_bucket();
    return 0;
}