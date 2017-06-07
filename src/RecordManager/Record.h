//
// Created by Haoxiangpeng on 6/6/2017.
//

#pragma once

#include <array>
#include "../CatalogManager/Catalog.h"
#include <cstring>

struct FixString;
class Catalog;

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

