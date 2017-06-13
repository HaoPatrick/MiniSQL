//
// Created by Haoxiangpeng on 6/10/2017.
//

#pragma once

#include <iostream>
#include <vector>
#include "../CatalogManager/Catalog.h"
#include "../BufferManager/FileHandler.h"
//#include <boost/filesystem.hpp>

//namespace fs=boost::filesystem;


class API {
public:
    void create_index(char *a, char *b, char *c) {
        std::cout << a << b << c << std::endl;
    }

    void create_table(std::string table_name,
                      std::vector<unsigned int> type_count,
                      std::vector<std::string> attr_names);

    Catalog load_table(std::string table_name);


    void create_index(std::string index_name,
                      std::string table_name,
                      std::string column_name);

    Record search_item(std::string table_name, std::string column_name, int int_value);

    Record search_item(std::string table_name, std::string column_name, float float_value);

    Record search_item(std::string table_name, std::string column_name, std::string char_value);

};

