#include <cctype>
#include <fstream>
#include <cassert>

#include "mc_driver.hpp"

MC::MC_Driver::~MC_Driver() {
    delete (scanner);
    scanner = nullptr;
    delete (parser);
    parser = nullptr;
}

void MC::MC_Driver::parse(const char *const filename) {
    assert(filename != nullptr);
    std::ifstream in_file(filename);
    if (!in_file.good()) {
        exit(EXIT_FAILURE);
    }
    parse_helper(in_file);
    return;
}

void MC::MC_Driver::parse(std::istream &stream) {
    if (!stream.good() && stream.eof()) {
        return;
    }
    //else
    parse_helper(stream);
    return;
}

void MC::MC_Driver::parse_helper(std::istream &stream) {

    delete (scanner);
    try {
        scanner = new MC::MC_Scanner(&stream);
    }
    catch (std::bad_alloc &ba) {
        std::cerr << "Failed to allocate scanner: (" << ba.what() << "), exiting!!\n";
        exit(EXIT_FAILURE);
    }

    delete (parser);
    try {
        parser = new MC::MC_Parser((*scanner) /* scanner */,
                                   (*this) /* driver */);
    }
    catch (std::bad_alloc &ba) {
        std::cerr << "Failed to allocate parser: (" << ba.what() << "), exiting!!\n";
        exit(EXIT_FAILURE);
    }
    const int accept(0);
    if (parser->parse() != accept) {
        std::cout << "Parse failed!!\n";
    }
    return;
}


void MC::MC_Driver::create_index(std::string index_name, std::string table_name, std::string column_name) {
    std::cout << "index " << index_name << " created." << std::endl;
}

void MC::MC_Driver::drop_index(std::string index_name) {
    std::cout << "index " << index_name << " dropped." << std::endl;
}

void MC::MC_Driver::drop_table(std::string table_name) {
    std::cout << "table " << table_name << " dropped" << std::endl;
}

void MC::MC_Driver::emit(const std::string &word) {
//     std::cout << word << std::endl;
}


std::ostream &
MC::MC_Driver::print(std::ostream &stream) {
    stream << red << "Down." << norm << "\n";
    return (stream);
}

void MC::MC_Driver::add_column(std::string name, attr_type type) {
    this->column_names.push_back(name);
    this->types.push_back(type);
}

void MC::MC_Driver::clear_all() {
    this->column_names.clear();
    this->types.clear();
    this->table_name = "";
    this->column_name = "";
    this->float_values.clear();
    this->int_values.clear();
    this->string_values.clear();
    this->where_cloumn = "";
    this->where_operator = "";
    this->where_value = "";
    this->is_select_all = false;
    this->has_compare = false;
    this->compare_func = -1;
}

void MC::MC_Driver::add_column(std::string name) {
    this->column_names.push_back(name);
}

void MC::MC_Driver::add_column_type(int type) {
    attr_type real_type;
    if (type == 40000) {
        real_type = attr_int;
    } else if (type == 90000) {
        real_type = attr_float;
    } else if (type == 120000) {
        real_type = attr_char;
    } else {
        real_type = attr_int;
    }
    this->types.push_back(real_type);
}

void MC::MC_Driver::debug_info() {
    std::cout << "Table name: " + table_name + " - Column name:" + column_name + "\n";
    std::cout << "column names: ";
    for (auto value:this->column_names) {
        std::cout << value + " ";
    }
    std::cout << " | ";
    std::cout << "type names: ";
    for (auto value:this->types) {
        std::cout << value << " ";
    }
    std::cout << " | ";
    std::cout << "values:";
    for (auto value:this->int_values) {
        std::cout << value << " ";
    }
    for (auto value:this->float_values) {
        std::cout << value << " ";
    }
    for (auto value:this->string_values) {
        std::cout << value << " ";
    }

    std::cout << "\n | ";
    std::cout << "where " + this->column_name + " " << this->compare_func;;

    std::cout << std::endl;
//    column_names.data() << " - " << types.data() << std::endl;
}

void MC::MC_Driver::create_table() {
    API table(this->table_name);
    std::vector<unsigned int> type_count;
    unsigned int int_count = (unsigned int) std::count(
            this->types.begin(), this->types.end(), attr_int);
    unsigned int float_count = (unsigned int) std::count(
            this->types.begin(), this->types.end(), attr_float);
    unsigned int string_count = (unsigned int) std::count(
            this->types.begin(), this->types.end(), attr_char);
    type_count.push_back(int_count);
    type_count.push_back(float_count);
    type_count.push_back(string_count);
    table.create_table(this->table_name, type_count, this->column_names);
    // Debug
//    this->show_table_info(this->table_name);
}

void MC::MC_Driver::show_table_info(std::string table_name) {
    API table(table_name);
    Catalog catalog = table.load_table(table_name);
    std::cout << catalog.table_name.to_string();
    for (auto item :catalog.attr_names) {
        std::cout << item.to_string() + " ";
    }
    std::cout << std::endl;
}

void MC::MC_Driver::insert_value() {
    API table(this->table_name);
    table.insert_value(this->table_name,
                       this->int_values,
                       this->float_values,
                       this->string_values);
//    std::cout<<"OK";
}

void MC::MC_Driver::execute_select() {
    API table(this->table_name);
    std::vector<Record> result;
    if (this->column_name != "") {
        std::vector<std::pair<Record, int>> temp_result;
        temp_result = table.select_all(
                column_name, compare_func, int_values, float_values, string_values);
        for (auto item:temp_result) {
            result.push_back(item.first);
        }
    } else {
        result = table.select_all();
    }

    this->print_them(result);
}

void MC::MC_Driver::print_them(std::vector<Record> results) {
    for (auto item :results) {
        std::cout << "| ";
        for (auto int_item:item.int_v) {
            std::cout << int_item << " | ";
        }
        for (auto float_item:item.float_v) {
            std::cout << float_item << " | ";
        }
        for (auto string_item:item.char_v) {
            std::cout << string_item.to_string() << " | ";
        }
        std::cout << std::endl;
    }
}

void MC::MC_Driver::execute_delete() {
    API table(this->table_name);
    table.delete_value(column_name, compare_func, int_values, float_values, string_values);
}
