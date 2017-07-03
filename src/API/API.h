//
// Created by Haoxiangpeng on 6/10/2017.
//

#pragma once

#include <iostream>
#include <vector>
#include "../CatalogManager/Catalog.h"
#include "../BufferManager/FileHandler.h"
#include "../RecordManager/Record.h"
//#include <boost/filesystem.hpp>

//namespace fs=boost::filesystem;


class API {
public:
    API(std::string table_name);

    void create_index(char *a, char *b, char *c) {
        std::cout << a << b << c << std::endl;
    }

    void create_table(std::string table_name,
                      std::vector<unsigned int> type_count,
                      std::vector<std::string> attr_names);

    void insert_value(std::string table_name,
                      std::vector<int> int_values,
                      std::vector<float> float_values,
                      std::vector<std::string> string_value);

    void delete_value(std::string column_name,
                      int operation,
                      std::vector<int> int_values,
                      std::vector<float> float_values,
                      std::vector<std::string> string_values);

    Catalog load_table(std::string table_name);


    void create_index(std::string index_name,
                      std::string table_name,
                      std::string column_name);

    std::vector<Record> select_all();

    std::vector<std::pair<Record, int>> select_all(std::string column_name,
                                   int operation,
                                   std::vector<int> int_values,
                                   std::vector<float> float_values,
                                   std::vector<std::string> string_values);

    Record search_item(std::string table_name, std::string column_name, int int_value);

    Record search_item(std::string table_name, std::string column_name, float float_value);

    Record search_item(std::string table_name, std::string column_name, std::string char_value);

private:
    std::string table_name;
};

