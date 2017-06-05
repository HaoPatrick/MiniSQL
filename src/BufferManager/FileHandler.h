//
// Created by Haoxiangpeng on 6/3/2017.
//

#pragma once

#include <iostream>
#include <fstream>
#include "../IndexManager/BTree.h"

#define BLOCK_SIZE 4096
#define FILE_PATH "db.hlh"
#define INDEX_PATH "db.idx"

enum FileType {
    table,
    index,
    catalog
};

struct DBHeader {
    uint8_t ultimate_value;
    uint32_t count;
    char db_name[32];
    FileType type;
    uint32_t item_size;
    int ava_slot[32];
};

struct SampleRecord {
    uint32_t index;
    char title[32];
    uint32_t views;
    char comment[32];
};

class FileHandler {
public:
    FileHandler(char *);

    FileHandler();

    ~FileHandler() {
        in_file.close();
        out_file.close();
    }

    DBHeader get_header() {
        return DB_file_header;
    }

    bool is_valid() {
        return valid;
    }

    std::string read_data(uint32_t, SampleRecord &record);

    BTree<int> build_tree();

    void write_tree(BTree<int>);

    void load_tree(BTree<int> &);

    void write_sample_data(DBHeader &, SampleRecord &);

    void append_data(SampleRecord, DBHeader);

private:
    DBHeader DB_file_header;
    bool valid;
    std::ifstream in_file;
    std::ofstream out_file;
};

