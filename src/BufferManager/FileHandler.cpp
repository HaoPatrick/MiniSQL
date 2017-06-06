//
// Created by Haoxiangpeng on 6/3/2017.
//

#include <cstring>
#include "FileHandler.h"

FileHandler::FileHandler(std::string file_path) : in_file(file_path, std::ios::binary),
                                                  out_file(file_path,
                                                           std::ios::binary | std::ios::app | std::ios::out) {
    in_file.read(reinterpret_cast<char *>(&DB_file_header), sizeof(DB_file_header));

    this->file_path = file_path;
}

FileHandler::FileHandler() {
    out_file.clear();
    strncpy(DB_file_header.db_name, "DB HLH", sizeof(DB_file_header.db_name) - 1);
    out_file.write(reinterpret_cast<char *>(&this->DB_file_header), sizeof(DB_file_header));
}


BTree<int> FileHandler::build_tree() {
    BTree<int> tree(3);
    in_file.clear();
    in_file.seekg(sizeof(DB_file_header), std::ios::beg);
    SampleRecord record;
    for (auto i = 0; i < DB_file_header.count; i++) {
        in_file.read(reinterpret_cast<char *>(&record), sizeof(record));
        tree.insert(std::pair<int, int>(record.views, i));
    }
    return tree;
}

void FileHandler::write_tree(BTree<int> b_tree) {
    std::vector<std::pair<int, int>> result;
    b_tree.traverse(result);
    DBHeader index_header;
    index_header.count = (uint32_t) result.size();
    index_header.type = index;
    std::ofstream out_file(INDEX_PATH, std::ios::binary);
    out_file.write(reinterpret_cast<char *>(&index_header), sizeof(index_header));
    out_file.write(reinterpret_cast<char *>(result.data()), sizeof(result[0]) * result.size());
    out_file.close();
}

void FileHandler::load_tree(BTree<int> &b_tree) {
    std::vector<std::pair<int, int>> result;
    DBHeader index_header;

    std::ifstream in_file(INDEX_PATH, std::ios::binary);
    in_file.read(reinterpret_cast<char *>(&index_header), sizeof(index_header));
    if (index_header.type != index || index_header.count <= 0) {
        return;
    }
    result.resize(index_header.count);
    in_file.read(reinterpret_cast<char *>(result.data()),
                 sizeof(result[0]) * index_header.count);
    in_file.close();
    for (auto item:result) {
        b_tree.insert(item);
    }
}

void FileHandler::write_sample_data(DBHeader &test_header, SampleRecord &test_data) {
    std::ofstream out_file(this->file_path, std::ios::binary);
    out_file.write(reinterpret_cast<char *>(&test_header), sizeof(test_header));

    for (auto i = 0; i < 30; i++) {
        test_data.views = (uint32_t) i;
        test_data.index = (uint32_t) i * 3;
        out_file.write(reinterpret_cast<char *>(&test_data), sizeof(test_data));
    }
}

std::string FileHandler::read_data(uint32_t index, SampleRecord &record) {
    std::string result;

    in_file.clear();
    in_file.seekg(0, std::ios::beg);
    DBHeader header;
    in_file.read(reinterpret_cast<char *>(&header), sizeof(header));

    in_file.seekg(index * sizeof(record) + sizeof(header));
    in_file.read(reinterpret_cast<char * >(&record), sizeof(record));
    result += (std::string) record.title + " " + std::to_string(record.views) + " " + std::to_string(record.index);
    return result;
}

void FileHandler::append_data(SampleRecord record, DBHeader header) {
    if (DB_file_header.type != header.type) {
        return;
    }
    out_file.clear();
    out_file.write(reinterpret_cast<char *>(&record), sizeof(record));
    out_file.close();

    std::ofstream rewrite_file(this->file_path, std::ios::binary | std::ios::in | std::ios::out);
    DBHeader new_header = header;
    new_header.count += 1;
    rewrite_file.clear();
    rewrite_file.seekp(0, std::ios::beg);
    rewrite_file.write(reinterpret_cast<char *>(&new_header), sizeof(new_header));
    rewrite_file.close();
}

void FileHandler::append_data(Record record) {
    out_file.clear();
    out_file.write(reinterpret_cast<char *>(record.int_v.data()),
                   sizeof(int) * record.int_v.size());
    out_file.write(reinterpret_cast<char *>(record.float_v.data()),
                   sizeof(float) * record.float_v.size());
    out_file.write(reinterpret_cast<char *>(record.char_v.data()),
                   sizeof(char[255]) * record.char_v.size());

    std::ofstream rewrite_file(this->file_path, std::ios::binary | std::ios::in | std::ios::out);
    DBHeader new_header = DB_file_header;
    new_header.count += 1;
    rewrite_file.clear();
    rewrite_file.seekp(0, std::ios::beg);
    rewrite_file.write(reinterpret_cast<char *>(&new_header), sizeof(new_header));
    rewrite_file.close();
}
