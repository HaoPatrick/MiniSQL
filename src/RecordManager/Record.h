//
// Created by Haoxiangpeng on 6/6/2017.
//

#pragma once

#include "../CatalogManager/Catalog.h"

class Record {
public:
    Record(Catalog);

    std::vector<int> int_v;
    std::vector<float> float_v;
    std::vector<char[255]> char_v;
    std::string table_name;
};

