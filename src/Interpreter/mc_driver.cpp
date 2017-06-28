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
        std::cerr << "Parse failed!!\n";
    }
    return;
}

void MC::MC_Driver::add_upper() {
    uppercase++;
    chars++;
    words++;
}

void MC::MC_Driver::create_index(std::string index_name, std::string table_name, std::string column_name) {
    std::cout << "create index " << index_name << " on " << table_name << "(" << column_name << ")" << std::endl;
}

void MC::MC_Driver::drop_index(std::string index_name) {
    std::cout << "index " << index_name << " dropped" << std::endl;
}

void MC::MC_Driver::drop_table(std::string table_name) {
    std::cout << "table " << table_name << " dropped" << std::endl;
}

void MC::MC_Driver::emit(const std::string &word) {
    std::cout << word << std::endl;
}

void MC::MC_Driver::add_lower() {
    lowercase++;
    chars++;
    words++;
}

void MC::MC_Driver::add_word(const std::string &word) {
    words++;
    chars += word.length();
    for (const char &c : word) {
        if (islower(c)) {
            lowercase++;
        } else if (isupper(c)) {
            uppercase++;
        }
    }
}

void MC::MC_Driver::add_newline() {
    lines++;
    chars++;
}

void MC::MC_Driver::add_char() {
    chars++;
}

std::ostream &
MC::MC_Driver::print(std::ostream &stream) {
    stream << red << "Results: " << norm << "\n";
    stream << blue << "Uppercase: " << norm << uppercase << "\n";
    stream << blue << "Lowercase: " << norm << lowercase << "\n";
    stream << blue << "Lines: " << norm << lines << "\n";
    stream << blue << "Words: " << norm << words << "\n";
    stream << blue << "Characters: " << norm << chars << "\n";
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
    std::cout << "type names: ";
    for (auto value:this->types) {
        std::cout << value << " ";
    }

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
    std::cout << std::endl;
//    column_names.data() << " - " << types.data() << std::endl;
}
