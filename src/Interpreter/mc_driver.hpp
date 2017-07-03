#ifndef __MCDRIVER_HPP__
#define __MCDRIVER_HPP__ 1

#include <string>
#include <cstddef>
#include <istream>
#include <vector>

#include "../API/API.h"
#include "../CatalogManager/Catalog.h"
#include "mc_scanner.hpp"
#include "mc_parser.tab.hh"

namespace MC {

    class MC_Driver {
    public:
        MC_Driver() = default;

        virtual ~MC_Driver();

        /**
         * parse - parse from a file
         * @param filename - valid string with input file
         */
        void parse(const char *const filename);

        /**
         * parse - parse from a c++ input stream
         * @param is - std::istream&, valid input stream
         */
        void parse(std::istream &iss);

        void add_upper();

        void add_lower();

        void add_word(const std::string &word);

        void add_newline();

        void add_char();

        void add_column(std::string, attr_type);

        void clear_all();

        void set_table_name(std::string name) {
            this->table_name = name;
        }

        void set_column_name(std::string name) {
            this->column_name = name;
        }

        void add_value(std::string value) {
            this->string_values.push_back(value);
        }

        void add_value(int value) {
            this->int_values.push_back(value);
        }

        void add_value(float value) {
            this->float_values.push_back(value);
        }

        void add_column(std::string);

        void add_column_type(int);

        void create_index(std::string index_name, std::string table_name, std::string column_name);

        void drop_index(std::string index_name);

        void drop_table(std::string table_name);

        void emit(const std::string &word);

        void debug_info();

        std::ostream &print(std::ostream &stream);

        void create_table();

        void insert_value();

        void set_select_all(bool value) {
            this->is_select_all = value;
        }

        void set_compare_func(int compare_func) {
            this->has_compare = true;
            this->compare_func = compare_func;
        }

        void execute_select();
        
        void execute_delete();

        void print_them(std::vector<Record> results);

        void show_table_info(std::string table_name);

    private:

        void parse_helper(std::istream &stream);

        std::vector<std::string> column_names;
        std::vector<attr_type> types;
        std::string table_name;
        std::string column_name;

        std::vector<std::string> string_values;
        std::vector<int> int_values;
        std::vector<float> float_values;

        std::string where_cloumn;
        std::string where_operator;
        std::string where_value;
        bool is_select_all;
        bool has_compare;
        int compare_func;

        MC::MC_Parser *parser = nullptr;
        MC::MC_Scanner *scanner = nullptr;

        const std::string red = "\033[1;31m";
        const std::string blue = "\033[1;36m";
        const std::string norm = "\033[0m";
    };

} /* end namespace MC */
#endif /* END __MCDRIVER_HPP__ */
