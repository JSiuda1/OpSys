#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string.h>
#include <boost/algorithm/string.hpp>
#include "csv_file.hh"

CSV_File::CSV_File(std::string file_name, const char* delimiter) {
    if (file_name.empty() == true) {
        throw std::invalid_argument("File name is empty");
    }

    copy_delimiter(delimiter);
    read_data(file_name.c_str());

}

CSV_File::CSV_File(const char *file_name, const char* delimiter) {
    if (strlen(file_name) == 0) {
        throw std::invalid_argument("File name is empty");
    }

    copy_delimiter(delimiter);
    read_data(file_name);
}

void CSV_File::copy_delimiter(const char *delimiter) {
    size_t delimiter_size = strlen(delimiter);

    this->delimiter = std::make_unique<char[]>(delimiter_size);
    memcpy(this->delimiter.get(), delimiter, delimiter_size);
}

void CSV_File::read_data(const char* file_name) {
    std::fstream file;
    file.open(file_name, std::ios::in);

    if (file.fail()) {
        throw std::runtime_error("Could not open file");
    }

    std::string input;
    std::vector<std::string> splited_input;
    while (std::getline(file, input, '\n')) {
        boost::split(splited_input, input, boost::is_any_of(this->delimiter.get()));
        this->rows.push_back(splited_input);
    }
}

std::vector<std::string> CSV_File::get_column_by_number(unsigned int column_number) {
    if (rows.size() == 0) {
        throw std::runtime_error("Data has 0 rows");
    }

    if (rows[0].size() < column_number) {
        throw std::runtime_error("Column number is greater than size");
    }

    std::vector<std::string> result;
    for(std::vector<std::string> row : rows) {
        result.push_back(row[column_number]);
    }

    return result;
}

size_t CSV_File::get_rows_number(void) {
    if (rows.size() == 0) {
        return 0;
    }

    return rows[0].size() + 1;
}