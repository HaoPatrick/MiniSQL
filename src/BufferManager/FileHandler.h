//
// Created by Haoxiangpeng on 6/3/2017.
//

#pragma once

#include <iostream>
#include <fstream>
#include "../IndexManager/BTree.h"
#include "../RecordManager/Record.h"
#include "../CatalogManager/Catalog.h"
#include <cstring>

#define BLOCK_SIZE 4096
#define INDEX_PATH "db.idx"

enum FileType {
    table,
    index,
    catalog
};


struct DBHeader {
//    uint8_t ultimate_value;
    uint32_t count;
    char db_name[32];
    FileType type;
    unsigned int int_count;
    unsigned int float_count;
    unsigned int char_count;
    unsigned int check_value;
};

struct SampleRecord {
    uint32_t index;
    char title[32];
    uint32_t views;
    char comment[32];
};

class FileHandler {
public:
    FileHandler(std::string);

    FileHandler(std::string, DBHeader);

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

    std::string search_data();

    std::string read_data(unsigned, Record &record);

    BTree<int> build_tree();

    void write_tree(BTree<int>);

    void load_tree(BTree<int> &);

    void write_catalog(Catalog);

    Catalog load_catalog();

    void write_sample_data(DBHeader &, SampleRecord &);

    void write_sample_data(Record);

    void append_data(SampleRecord, DBHeader);

    void append_data(Record);

private:
    DBHeader DB_file_header;
    bool valid;
    std::string file_path;
    std::ifstream in_file;
    std::ofstream out_file;
};



