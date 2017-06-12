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
    FileHandler file("catalog_" + table_name + ".hlh");
    Catalog result = file.load_catalog();
    return result;
}

void API::create_index(std::string index_name,
                       std::string table_name,
                       std::string column_name) {

}
