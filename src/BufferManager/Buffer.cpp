//
// Created by Haoxiangpeng on 6/3/2017.
//

#include <cstring>
#include "Buffer.h"

Buffer::Buffer(std::string file_path) {
    std::ifstream in_file(file_path, std::ios::binary);
    in_file.read(reinterpret_cast<char *>(&DB_file_header), sizeof(DB_file_header));
    if (DB_file_header.header1 != 42) {
        this->valid = false;
    }
    std::cout << DB_file_header.db_name << std::endl;
}

Buffer::Buffer() {
    std::ofstream out_file(FILE_PATH, std::ios::binary);
    DB_file_header.header1 = 42;
    strncpy(DB_file_header.db_name, "DB HLH", sizeof(DB_file_header.db_name) - 1);
    out_file.write(reinterpret_cast<char *>(&this->DB_file_header), sizeof(DB_file_header));
    out_file.close();
}

void Buffer::write_sample_data() {
    std::ofstream out_file(FILE_PATH, std::ios::binary);
    SampleRecord test_data;
    strncpy(test_data.title, "Great wbx", sizeof(test_data.title) - 1);
    strncpy(test_data.comment, "Hey wbx", sizeof(test_data.comment) - 1);
    test_data.index = 42;
    for (auto i = 0; i < 100; i++) {
        test_data.views = (uint32_t) i;
        out_file.write(reinterpret_cast<char *>(&test_data), sizeof(test_data));
    }
    out_file.close();
}

void Buffer::read_sample_data() {
    std::ifstream in_file(FILE_PATH, std::ios::binary);
    SampleRecord record;
    for (auto i = 0; i < 100; i++) {
        in_file.read(reinterpret_cast<char *>(&record), sizeof(record));
        std::cout << record.title << record.comment << record.views << record.index << std::endl;
    }
    in_file.close();
}

void Buffer::read_sample_data(uint32_t index) {
    std::ifstream in_file(FILE_PATH, std::ios::binary);
    in_file.seekg(index * sizeof(SampleRecord));
    SampleRecord record;
    in_file.read(reinterpret_cast<char * >(&record), sizeof(record));
    std::cout << record.title << record.views << std::endl;
    in_file.close();
}