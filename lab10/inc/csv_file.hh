#include <string>
#include <vector>


#pragma once



class CSV_File {
    private:
    std::vector<std::vector<std::string>> rows;
    std::unique_ptr<char[]> delimiter;

    public:
    CSV_File(std::string file_name, const char* delimiter);
    CSV_File(const char* file_name, const char* delimiter);
    std::vector<std::string> get_column_by_number(unsigned int coulmn_number);
    size_t get_rows_number(void);

    protected:
    void read_data(const char* file_name);
    void copy_delimiter(const char* delimiter);
};