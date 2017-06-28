#ifndef __MCDRIVER_HPP__
#define __MCDRIVER_HPP__ 1

#include <string>
#include <cstddef>
#include <istream>
#include <vector>

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

        void clear_table_info();

        void set_table_name(std::string name) {
            this->table_name = name;
        }

        void add_column(std::string);

        void add_column_type(int);

        void create_index(std::string index_name, std::string table_name, std::string column_name);

        void drop_index(std::string index_name);

        void drop_table(std::string table_name);

        void emit(const std::string &word);

        void debug_info();

        std::ostream &print(std::ostream &stream);

    private:

        void parse_helper(std::istream &stream);

        std::vector<std::string> column_names;
        std::vector<attr_type> types;
        std::string table_name;
        std::string column_name;

        std::size_t chars = 0;
        std::size_t words = 0;
        std::size_t lines = 0;
        std::size_t uppercase = 0;
        std::size_t lowercase = 0;
        MC::MC_Parser *parser = nullptr;
        MC::MC_Scanner *scanner = nullptr;

        const std::string red = "\033[1;31m";
        const std::string blue = "\033[1;36m";
        const std::string norm = "\033[0m";
    };

} /* end namespace MC */
#endif /* END __MCDRIVER_HPP__ */
