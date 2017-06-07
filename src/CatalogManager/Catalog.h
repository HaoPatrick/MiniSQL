//
// Created by Haoxiangpeng on 6/6/2017.
//

#pragma once


#include <cstdint>
#include <string>
#include <vector>
#include <cstring>
#include "../RecordManager/Record.h"

enum attr_type {
    attr_int,
    attr_float,
    attr_char,
    error
};

struct FixString {
    FixString(std::string source_value) {
        strncpy(this->value, source_value.c_str(), sizeof(char) * 255);
    }

    FixString() {
        memset(value, 0, sizeof(char) * 255);
    }

    FixString operator=(const std::string &outer) {
        return FixString(outer);
    }

    char value[255];
};

class Catalog {
public:
    uint32_t int_count;
    uint32_t float_count;
    uint32_t char_count;
    std::vector<FixString> attr_names;

    std::string table_name;

    size_t size();

    ptrdiff_t get_pos(std::string);

    attr_type query_type(std::string);

    Catalog();

    Catalog(unsigned int, unsigned int, unsigned int);

    void set_attr_name(std::vector<FixString>);

    void set_table_name(std::string);

};


