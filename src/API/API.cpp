//
// Created by Haoxiangpeng on 6/10/2017.
//

#include "API.h"

void API::create_table(std::string table_name,
                       std::vector<unsigned int> type_count,
                       std::vector<std::string> attr_names) {
    FileHandler file("catalog_" + table_name + ".hlh");
    Catalog catalog(type_count[0], type_count[1], type_count[2]);
    catalog.table_name = table_name;
    for (auto i = 0; i < type_count[0] + type_count[1] + type_count[2]; i++) {
        catalog.attr_names[i] = attr_names[i];
    }
    file.write_catalog(catalog);
}

Catalog API::load_table(std::string table_name) {
//    if (!boost::filesystem::exists("catalog_" + table_name + ".hlh")) {
//        return Catalog();
//    }
    FileHandler file("catalog_" + table_name + ".hlh");
    // TODO: check if exist
    Catalog result = file.load_catalog();
    return result;
}

void API::create_index(std::string index_name,
                       std::string table_name,
                       std::string column_name) {
    Catalog catalog = load_table(table_name);
    attr_type column_type = catalog.query_type(column_name);
    if (column_type != attr_int) {
        return;
    }
    Record sample_record(catalog);
    unsigned int int_pos = (unsigned int) catalog.get_pos(column_name);
    FileHandler table_file("table_" + table_name + ".hlh", FileType(table_type));
    BTree<int> tree = table_file.build_tree(int_pos, sample_record);

    FileHandler index_file("index_" + index_name + ".hlh",
                           FileType(index_type));
    index_file.write_tree(tree);
}



