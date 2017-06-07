#include <cstring>
#include "BufferManager/FileHandler.h"

int main(int argc, char *argv[]) {
    DBHeader test_header;
    strncpy(test_header.db_name, "HLH DB", sizeof(test_header.db_name) - 1);
    test_header.type = table;
    test_header.count = 30;
    test_header.int_count = 2;
    test_header.float_count = 1;
    test_header.char_count = 1;
    test_header.check_value = 42;
    FileHandler aa("new.hlh", test_header);

//    Catalog catalog(2, 1, 1);
//    catalog.table_name = "test table";
//    catalog.attr_names[0] = "int 0";
//    catalog.attr_names[1] = "int 1";
//    catalog.attr_names[2] = "float 0";
//    catalog.attr_names[3] = "char 0";

//    Record record(catalog);
//    record.int_v[0] = 42;
//    record.int_v[1] = 24;
//    record.float_v[0] = 3.1415;
//    strncpy(record.char_v[0], "hello hlh!", sizeof(record.char_v[0])-1);
//    record.char_v[0] = "hello hlh!";

//
//    aa.write_sample_data(record);
//
//    Record result_record(catalog);
//    std::string result_string = aa.read_data(3, record);
//    std::cout << result_string << std::endl;

    return 0;
}