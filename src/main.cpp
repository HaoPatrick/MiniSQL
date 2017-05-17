#include "BPlusTree.h"


int main(int argc, char *argv[]) {
    Bucket bpt(4);
    Node test_node;
    test_node.pointers = nullptr;
    bpt.type = root;
    test_node.value = std::make_pair(3, "hello");
    bpt.insert_to_cur_bucket(test_node);
    test_node.value = std::make_pair(4, "hello");
    bpt.insert_to_cur_bucket(test_node);
    test_node.value = std::make_pair(5, "hello");
    bpt.insert_to_cur_bucket(test_node);

    Bucket result = bpt.find_bucket(test_node);
    result.print_bucket();
    return 0;
}