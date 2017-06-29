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
        strncpy(this->value, source_value.c_str(), 255);
    }

    FixString() {
        memset(value, 0, 255);
    }

    std::string to_string() {
        return std::string(this->value);
    };

    void operator=(const std::string &outer) {
        strncpy(this->value, outer.c_str(), 255);
    }

    bool operator==(const FixString &outer) {
        return strncmp(this->value, outer.value, 255) == 0;
    }

    bool operator>(const std::string &outer) {
        return std::string(this->value) > outer;
    }

    bool operator>=(const std::string &outer) {
        return std::string(this->value) >= outer;
    }

    bool operator<(const std::string &outer) {
        return std::string(this->value) < outer;
    }

    bool operator<=(const std::string &outer) {
        return std::string(this->value) <= outer;
    }

    bool operator==(const char *outer) {
        return strncmp(this->value, outer, 255) == 0;
    }

    bool operator==(const std::string &outer) {
        return outer == this->value;
    }

    char value[255];
};

class Catalog {
public:
    unsigned int int_count;
    unsigned int float_count;
    unsigned int char_count;

//    unsigned int deleted_index[100];
    std::vector<FixString> attr_names;

    unsigned int delete_count;
    std::vector<int> deleted_pos;

    FixString table_name;

    size_t size();

    ptrdiff_t get_pos(FixString);

    ptrdiff_t get_pos(std::string);

    attr_type query_type(std::string);

    Catalog();

    Catalog(unsigned int, unsigned int, unsigned int);

    void set_attr_name(std::vector<FixString>);

    void set_table_name(std::string);

//    std::istream &operator>>(std::istream &is, Catalog &cat);
};



