//
// Created by Haoxiangpeng on 6/3/2017.
//

#pragma once

#include <iostream>
#include <fstream>

#define BLOCK_SIZE 4096
#define FILE_PATH "db.hlh"

struct TempHeader {
    uint8_t header1;
    char db_name[32];
    uint8_t header3;
    uint8_t header4;
};

struct SampleRecord {
    uint32_t index;
    char title[32];
    uint32_t views;
    char comment[32];
};

class Buffer {
public:
    Buffer(std::string);

    Buffer();

    TempHeader get_header() {
        return DB_file_header;
    }

    bool is_valid() {
        return valid;
    }

    void read_sample_data();

    void read_sample_data(uint32_t);

    void write_sample_data();

private:
    TempHeader DB_file_header;
    bool valid;
};

