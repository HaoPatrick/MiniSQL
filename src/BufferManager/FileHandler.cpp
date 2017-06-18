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

FileHandler::FileHandler(std::string file_path, FileType file_type) :
        in_file(file_path, std::ios::binary),
        out_file(file_path,
                 std::ios::binary | std::ios::app |
                 std::ios::out) {
    DBHeader file_header;
    file_header.count = 0;
    file_header.type = file_type;
    this->DB_file_header = file_header;
    out_file.write(reinterpret_cast<char *>(&this->DB_file_header), sizeof(this->DB_file_header));
    this->file_path = file_path;
}

FileHandler::FileHandler(std::string file_path, DBHeader db_header) :
        in_file(file_path, std::ios::binary),
        out_file(file_path,
                 std::ios::binary | std::ios::app |
                 std::ios::out) {
    this->DB_file_header = db_header;
    out_file.write(reinterpret_cast<char *>(&db_header), sizeof(db_header));
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

BTree<int> FileHandler::build_tree(unsigned int position,
                                   Record sample_record) {
    BTree<int> tree(3);
    in_file.clear();
    in_file.seekg(sizeof(DB_file_header), std::ios::beg);
    for (auto i = 0; i < DB_file_header.count; i++) {
        in_file.read(reinterpret_cast<char *>(&sample_record), sizeof(sample_record));
        tree.insert(std::pair<int, int>(sample_record.int_v[position], i));
    }
    return tree;
}

void FileHandler::write_tree(BTree<int> b_tree) {
    std::vector<std::pair<int, int>> result;
    b_tree.traverse(result);
    DBHeader index_header;
    index_header.count = (uint32_t) result.size();
    index_header.type = index;
    std::ofstream out_file(this->file_path, std::ios::binary);
    out_file.write(reinterpret_cast<char *>(&index_header), sizeof(index_header));
    out_file.write(reinterpret_cast<char *>(result.data()), sizeof(result[0]) * result.size());
    out_file.close();
}

void FileHandler::write_catalog(Catalog catalog) {
    std::ofstream new_file(file_path, std::ios::binary);
    new_file.write(reinterpret_cast<char *>(&DB_file_header), sizeof(DB_file_header));
    new_file.write(reinterpret_cast<char *>(
                           &catalog.int_count), sizeof(unsigned int));
    new_file.write(reinterpret_cast<char *>(
                           &catalog.float_count), sizeof(unsigned int));
    new_file.write(reinterpret_cast<char *>(
                           &catalog.char_count), sizeof(unsigned int));
    new_file.write(reinterpret_cast<char *>(
                           &catalog.table_name), sizeof(catalog.table_name));
    new_file.write(reinterpret_cast<char *>(
                           catalog.attr_names.data()), sizeof(FixString) * catalog.attr_names.size());

//    new_file.write(reinterpret_cast<char *>(&catalog), sizeof(catalog));
    new_file.close();
}

Catalog FileHandler::load_catalog() {
    Catalog catalog;
    this->in_file.clear();
    in_file.seekg(sizeof(DB_file_header));
    in_file.read(reinterpret_cast<char *>(
                         &catalog.int_count), sizeof(unsigned int));
    in_file.read(reinterpret_cast<char *>(
                         &catalog.float_count), sizeof(unsigned int));
    in_file.read(reinterpret_cast<char *>(
                         &catalog.char_count), sizeof(unsigned int));
    in_file.read(reinterpret_cast<char *>(
                         &catalog.table_name), sizeof(catalog.table_name));
    in_file.read(reinterpret_cast<char *>(
                         catalog.attr_names.data()), sizeof(FixString) * catalog.attr_names.size());

//    in_file.read(reinterpret_cast<char *>(&result), sizeof(result));
    return catalog;
}

void FileHandler::load_tree(BTree<int> &b_tree) {
    std::vector<std::pair<int, int>> result;

    DBHeader &index_header = this->DB_file_header;

    std::ifstream in_file(this->file_path, std::ios::binary);
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

void FileHandler::write_sample_data(Record record, int count) {
    std::ofstream out_file(this->file_path, std::ios::binary);
    this->DB_file_header.count = (uint32_t) count;
    out_file.write(reinterpret_cast<char *>(&DB_file_header), sizeof(DB_file_header));

    for (auto i = 0; i < count; i++) {
        record.int_v[0] = i;
        record.float_v[0] = i + (float) 0.01;
        out_file.write(reinterpret_cast<char *>(record.int_v.data()),
                       sizeof(int) * record.int_count);
        out_file.write(reinterpret_cast<char *>(record.float_v.data()),
                       sizeof(float) * record.float_count);
        out_file.write(reinterpret_cast<char *>(record.char_v.data()),
                       sizeof(char[255]) * record.char_count);
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

std::string FileHandler::read_data(unsigned int index, Record &record) {
    std::string result;
//    if (DB_file_header.delete_index.end() !=
//        std::find(DB_file_header.delete_index.begin(), DB_file_header.delete_index.end(), index)) {
//        return false;
//    }
    in_file.clear();
    in_file.seekg(index * (sizeof(int) * record.int_count +
                           sizeof(float) * record.float_count + sizeof(char) * 255 * record.char_count) +
                  sizeof(DB_file_header));
    in_file.read(reinterpret_cast<char *>(record.int_v.data()),
                 sizeof(int) * record.int_count);
    in_file.read(reinterpret_cast<char *>(record.float_v.data()),
                 sizeof(float) * record.float_count);
    in_file.read(reinterpret_cast<char *>(record.char_v.data()),
                 sizeof(char[255]) * record.char_count);

    for (auto i:record.int_v) {
        result += std::to_string(i) + ' ';
    }
    for (auto i:record.float_v) {
        result += std::to_string(i) + ' ';
    }
    for (auto i:record.char_v) {
        result += (std::string) i.value + ' ';
    }
    return result;
}

void FileHandler::append_data(SampleRecord record, DBHeader header) {
    if (DB_file_header.type != header.type) {
        return;
    }
    out_file.clear();
    out_file.write(reinterpret_cast<char *>(&record), sizeof(record));

    std::ofstream rewrite_file(this->file_path, std::ios::binary | std::ios::in | std::ios::out);
    DBHeader new_header = header;
    new_header.count += 1;
    rewrite_file.clear();
    rewrite_file.seekp(0, std::ios::beg);
    rewrite_file.write(reinterpret_cast<char *>(&new_header), sizeof(new_header));
    rewrite_file.close();
}

void FileHandler::insert_record(Record record) {
    out_file.clear();
    out_file.write(reinterpret_cast<char *>(record.int_v.data()),
                   sizeof(int) * record.int_count);
    out_file.write(reinterpret_cast<char *>(record.float_v.data()),
                   sizeof(float) * record.float_count);
    out_file.write(reinterpret_cast<char *>(record.char_v.data()),
                   sizeof(char[255]) * record.char_count);

    std::ofstream rewrite_file(this->file_path, std::ios::binary | std::ios::in | std::ios::out);
    DB_file_header.count += 1;
    rewrite_file.clear();
    rewrite_file.seekp(0, std::ios::beg);
    rewrite_file.write(reinterpret_cast<char *>(&DB_file_header), sizeof(DB_file_header));
    rewrite_file.close();
}

bool FileHandler::linear_search(
        Record &record, int pos, int value) {
    in_file.clear();
    in_file.seekg(sizeof(DB_file_header));
    for (auto i = 0; i < DB_file_header.count; i++) {
        in_file.read(reinterpret_cast<char *>(record.int_v.data()),
                     sizeof(int) * record.int_count);
        in_file.read(reinterpret_cast<char *>(record.float_v.data()),
                     sizeof(float) * record.float_count);
        in_file.read(reinterpret_cast<char *>(record.char_v.data()),
                     sizeof(char[255]) * record.char_count);
        if (record.int_v[pos] == value) {
            return true;
        }
    }
    return false;
}

bool FileHandler::linear_search(Record &record, int pos, float value) {
    in_file.clear();
    in_file.seekg(sizeof(DB_file_header));
    for (auto i = 0; i < DB_file_header.count; i++) {
        in_file.read(reinterpret_cast<char *>(record.int_v.data()),
                     sizeof(int) * record.int_count);
        in_file.read(reinterpret_cast<char *>(record.float_v.data()),
                     sizeof(float) * record.float_count);
        in_file.read(reinterpret_cast<char *>(record.char_v.data()),
                     sizeof(char[255]) * record.char_count);
        if (record.float_v[pos] == value) {
            return true;
        }
    }
    return false;
}

bool FileHandler::linear_search(Record &record, int pos, std::string value) {
    in_file.clear();
    in_file.seekg(sizeof(DB_file_header));
    for (auto i = 0; i < DB_file_header.count; i++) {
        in_file.read(reinterpret_cast<char *>(record.int_v.data()),
                     sizeof(int) * record.int_count);
        in_file.read(reinterpret_cast<char *>(record.float_v.data()),
                     sizeof(float) * record.float_count);
        in_file.read(reinterpret_cast<char *>(record.char_v.data()),
                     sizeof(char[255]) * record.char_count);
        if (record.char_v[pos] == value) {
            return true;
        }
    }
    return false;
}


std::vector<Record>
FileHandler::interval_search(
        int pos, int value, Record sample_record, std::function<bool(int, int)> func) {
    std::vector<Record> result;
    in_file.clear();
    in_file.seekg(sizeof(DB_file_header));
    for (auto i = 0; i < DB_file_header.count; i++) {
        Record current_record = sample_record;
        in_file.read(reinterpret_cast<char *>(current_record.int_v.data()),
                     sizeof(int) * current_record.int_count);
        in_file.read(reinterpret_cast<char *>(current_record.float_v.data()),
                     sizeof(float) * current_record.float_count);
        in_file.read(reinterpret_cast<char *>(current_record.char_v.data()),
                     sizeof(char[255]) * current_record.char_count);
        if (func(current_record.int_v[pos], value)) {
            result.push_back(current_record);
        }
    }
    return result;
}

std::vector<Record>
FileHandler::interval_search(
        int pos, float value, Record sample_record, std::function<bool(float, float)> func) {
    std::vector<Record> result;
    in_file.clear();
    in_file.seekg(sizeof(DB_file_header));
    for (auto i = 0; i < DB_file_header.count; i++) {
        Record current_record = sample_record;
        in_file.read(reinterpret_cast<char *>(current_record.int_v.data()),
                     sizeof(int) * current_record.int_count);
        in_file.read(reinterpret_cast<char *>(current_record.float_v.data()),
                     sizeof(float) * current_record.float_count);
        in_file.read(reinterpret_cast<char *>(current_record.char_v.data()),
                     sizeof(char[255]) * current_record.char_count);
        if (func(current_record.float_v[pos], value)) {
            result.push_back(current_record);
        }
    }
    return result;
}

std::vector<Record>
FileHandler::interval_search(int pos, std::string value, Record sample_record,
                             std::function<bool(FixString, std::string)> func) {
    std::vector<Record> result;
    in_file.clear();
    in_file.seekg(sizeof(DB_file_header));
    for (auto i = 0; i < DB_file_header.count; i++) {
        Record current_record = sample_record;
        in_file.read(reinterpret_cast<char *>(current_record.int_v.data()),
                     sizeof(int) * current_record.int_count);
        in_file.read(reinterpret_cast<char *>(current_record.float_v.data()),
                     sizeof(float) * current_record.float_count);
        in_file.read(reinterpret_cast<char *>(current_record.char_v.data()),
                     sizeof(char[255]) * current_record.char_count);
        if (func(current_record.char_v[pos], value)) {
            result.push_back(current_record);
        }
    }
    return result;
}


//std::string FileHandler::search_data(Record &record) {
//    std::string result;
//
//    in_file.clear();
//    in_file.seekg(index * (sizeof(int) * record.int_count +
//                           sizeof(float) * record.float_count + sizeof(char) * 255 * record.char_count) +
//                  sizeof(DB_file_header));
//    return std::string();
//}
