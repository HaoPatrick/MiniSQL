//
// Created by Haoxiangpeng on 5/17/2017.
//

#pragma once

#include <cstdio>
#include <algorithm>

namespace trees {
    template<class T>
    void insert_to_array(T *array, size_t sz, size_t insert_pos, const T &value) {
        for (auto i = sz - 1; i > insert_pos; i--) {
            std::swap(array[i], array[i - 1]);
        }
        array[insert_pos] = value;
    }
}