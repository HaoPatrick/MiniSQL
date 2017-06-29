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
#include <functional>
#include <algorithm>


enum FileType {
    table_type,
    index_type,
    catalog_type
};


struct DBHeader {
//    uint8_t ultimate_value;
    uint32_t count;
    char db_name[32];
    FileType type;

//    unsigned int int_count;
//    unsigned int float_count;
//    unsigned int char_count;
    unsigned int check_value;
    unsigned int delete_pos;

//    std::vector<unsigned int> delete_index;
//
//    DBHeader() {
//        this->delete_index.resize(100);
//    }
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

    FileHandler(std::string, FileType);

    FileHandler();

    void read_header();

    void write_header(DBHeader);

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

    std::string read_data(unsigned int, Record &record);

    bool linear_search(Record &record, int pos, int value);

    bool linear_search(Record &record, int pos, float value);

    bool linear_search(Record &record, int pos, std::string value);

    std::vector<Record> interval_search(int pos, int value, Record sample_record, std::function<bool(int, int)> func);

    std::vector<Record>
    interval_search(int pos, float value, Record sample_record, std::function<bool(float, float)> func);

    std::vector<Record>
    interval_search(int pos, std::string value, Record sample_record,
                    std::function<bool(FixString, std::string)> func);

    std::vector<Record> select_all(Record sample_record);

    BTree<int> build_tree();

    BTree<int> build_tree(unsigned int, Record);

    void write_tree(BTree<int>);

    void load_tree(BTree<int> &);

    void write_catalog(Catalog);

    Catalog load_catalog();

    void write_sample_data(DBHeader &, SampleRecord &);

    void write_sample_data(Record, int);

    void append_data(SampleRecord, DBHeader);

    void insert_record(Record);

private:
    DBHeader DB_file_header;
    bool valid;
    std::string file_path;
    std::ifstream in_file;
    std::ofstream out_file;


};

