//
// Created by Haoxiangpeng on 6/6/2017.
//

#include <algorithm>
#include "Catalog.h"


Catalog::Catalog() {
    this->int_count = 0;
    this->float_count = 0;
    this->char_count = 0;
    this->delete_count = 0;
    this->attr_names.clear();
    this->attr_names.resize(32);// we only need 32 attributes
    this->deleted_pos.resize(100);
    this->table_name = "";
}

Catalog::Catalog(unsigned int int_count, unsigned int float_count, unsigned int char_count) {
    this->int_count = int_count;
    this->float_count = float_count;
    this->char_count = char_count;
    this->delete_count = 0;
    if (int_count + float_count + char_count >= 32) {
        //TODO: attribute overflow
    }
    this->deleted_pos.resize(100);
    this->attr_names.resize(32);
}

void Catalog::set_attr_name(std::vector<FixString> names) {
    this->attr_names.resize(int_count + float_count + char_count);
    this->attr_names = names;
}

void Catalog::set_table_name(std::string table) {
    this->table_name = table;
}

size_t Catalog::size() {
    return sizeof(int) * this->int_count +
           sizeof(char) * 255 * this->char_count +
           sizeof(float_count) * this->char_count;
}

ptrdiff_t Catalog::get_pos(FixString attr_name) {
    return std::find(this->attr_names.begin(), this->attr_names.end(), attr_name) -
           this->attr_names.begin();
}

int Catalog::get_pos(std::string attr_name) {
//    attr_type column_type = this->query_type(attr_name);
    auto diff = std::find(this->attr_names.begin(), this->attr_names.end(),
                          attr_name) - this->attr_names.begin();
    if (diff < int_count) {
        return diff;
    } else if (diff < int_count + float_count) {
        return diff - int_count;
    } else if (diff < int_count + float_count + char_count) {
        return diff - int_count - float_count;
    } else {
        return -1;
    }

}

attr_type Catalog::query_type(std::string attr_name) {
    auto pos = std::find(this->attr_names.begin(), this->attr_names.end(),
                          attr_name) - this->attr_names.begin();
    attr_type result_type;
    if (pos >= 0 && pos < int_count) {
        result_type = attr_int;
    } else if (pos < int_count + float_count) {
        result_type = attr_float;
    } else if (pos < int_count + float_count + char_count) {
        result_type = attr_char;
    } else {
        result_type = error;
    }
    return result_type;
}

