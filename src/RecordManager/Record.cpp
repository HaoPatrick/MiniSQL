//
// Created by Haoxiangpeng on 6/6/2017.
//

#include "Record.h"


Record::Record(Catalog category) {
    this->int_v.resize(category.int_count);
    this->float_v.resize(category.float_count);
    this->char_v.resize(category.char_count);
    this->int_count = category.int_count;
    this->float_count = category.float_count;
    this->char_count = category.char_count;
}