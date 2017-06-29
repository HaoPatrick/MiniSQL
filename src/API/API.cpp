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

    DBHeader header;
    strncpy(header.db_name, table_name.c_str(), sizeof(header.db_name) - 1);
    header.type = table_type;
    header.count = 0;
    header.check_value = 42;
    FileHandler db_file("table_" + table_name + ".hlh", header);
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

void API::insert_value(std::string table_name, std::vector<int> int_values, std::vector<float> float_values,
                       std::vector<std::string> string_value) {
    Catalog catalog = this->load_table(table_name);
    if (catalog.int_count != int_values.size() ||
        catalog.float_count != float_values.size() ||
        catalog.char_count != string_value.size()) {
        return;
    }
    std::vector<FixString> fix_string_values;
    for (auto item:string_value) {
        fix_string_values.push_back(FixString(item));
    }
    Record item_to_add(catalog);
    item_to_add.int_v = int_values;
    item_to_add.float_v = float_values;
    item_to_add.char_v = fix_string_values;

    FileHandler db_file("table_" + table_name + ".hlh");
    db_file.insert_record(item_to_add);
    // Debug
//    std::cout<<"OK";
}

std::vector<Record> API::select_all() {
    std::vector<Record> result;
    FileHandler db_file("table_" + table_name + ".hlh");

    Catalog catalog = this->load_table(this->table_name);
    Record sample_record(catalog);
    result = db_file.select_all(sample_record);
    return result;
}

API::API(std::string table_name) {
    this->table_name = table_name;
}

