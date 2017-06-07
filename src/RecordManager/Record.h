//
// Created by Haoxiangpeng on 6/6/2017.
//

#pragma once

#include <array>
#include "../CatalogManager/Catalog.h"
#include <cstring>

struct FixString {
    FixString(std::string source_value) {
        strncpy(this->value, source_value.c_str(), sizeof(char) * 255);
    }

    FixString() {
        memset(value, 0, sizeof(char) * 255);
    }

    char value[255];
};

class Record {
public:
    Record(Catalog);

    std::vector<int> int_v;
    std::vector<float> float_v;
    std::vector<FixString> char_v;

    unsigned int int_count;
    unsigned int float_count;
    unsigned int char_count;
};

