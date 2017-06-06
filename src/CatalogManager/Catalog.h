//
// Created by Haoxiangpeng on 6/6/2017.
//

#pragma once


#include <cstdint>
#include <string>
#include <vector>

enum attr_type{
    attr_int,
    attr_float,
    attr_char,
    error
};

class Catalog {
public:
    uint32_t int_count;
    uint32_t float_count;
    uint32_t char_count;
    std::vector<std::string> attr_names;

    std::string table_name;

    size_t size();

    ptrdiff_t get_pos(std::string);

    attr_type query_type(std::string);

    Catalog();

};


