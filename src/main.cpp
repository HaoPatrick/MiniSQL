#include <cstring>
#include "BufferManager/Buffer.h"

int main(int argc, char *argv[]) {
    SampleRecord test_data;
    DBHeader test_header;
    strncpy(test_header.db_name, "HLH DB", sizeof(test_header.db_name) - 1);
    test_header.type = table;
    test_header.count = 30;
    test_header.item_size = sizeof(test_data);
    test_header.ultimate_value = 42;
    memset(test_header.ava_slot, 0, sizeof(test_header.ava_slot));

    strncpy(test_data.title, "Great wbx", sizeof(test_data.title) - 1);
    strncpy(test_data.comment, "Hey wbx", sizeof(test_data.comment) - 1);


    Buffer aa((char *) FILE_PATH);

//    aa.write_sample_data(test_header, test_data);
//    aa.append_data(test_data, test_header);
//    aa.read_data(30, test_data);
//
    BTree<int> tree = aa.build_tree();
//    tree.traverse();
//    aa.write_tree(tree);
    BTree<int> test_tree(3);
    aa.load_tree(test_tree);
    std::pair<int, int> *result_node = test_tree.search(29);
    if (result_node) {
        std::cout << result_node->first << result_node->second << std::endl;
    }

    return 0;
}