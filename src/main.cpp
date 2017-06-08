
#include "BufferManager/FileHandler.h"

int main(int argc, char *argv[]) {
    DBHeader test_header;
    strncpy(test_header.db_name, "HLH DB",
            sizeof(test_header.db_name) - 1);
    test_header.type = catalog;
    test_header.count = 1;
    test_header.int_count = 2;
    test_header.float_count = 1;
    test_header.char_count = 1;
    test_header.check_value = 42;
    FileHandler aa("catalog.hlh", test_header);

    Catalog catalog(2, 1, 1);
    catalog.table_name = "test table";
    catalog.attr_names[0] = "int 0";
    catalog.attr_names[1] = "int 1";
    catalog.attr_names[2] = "float 0";
    catalog.attr_names[3] = "char 0";

    aa.write_catalog(catalog);

    Catalog result_catalog = aa.load_catalog();

    std::cout << result_catalog.table_name.to_string() << result_catalog.int_count << std::endl;
    return 0;
}