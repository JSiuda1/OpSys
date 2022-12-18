#include <iostream>
#include <thread>
#include <list>
#include <signal.h>
#include <openssl/evp.h>
#include <list>
#include <string.h>
#include "csv_file.hh"
#include "user.hh"

// static void sighub_handler(int sig) {
//     printf("Hack summary: %d\n", sig);
//     exit(0);
// }

std::list<std::string> dictionary;
DataToHackCollection users_data_set;

static void bytes2md5(const char *data, int len, char *md5buf) {
	EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
	const EVP_MD *md = EVP_md5();
	unsigned char md_value[EVP_MAX_MD_SIZE];
	unsigned int md_len;
	EVP_DigestInit_ex(mdctx, md, NULL);
	EVP_DigestUpdate(mdctx, data, len);
	EVP_DigestFinal_ex(mdctx, md_value, &md_len);
	EVP_MD_CTX_free(mdctx);
	for (size_t i = 0; i < md_len; i++) {
		snprintf(&(md5buf[i * 2]), 16 * 2, "%02x", md_value[i]);
	}
}

void thread_one(void) {
    char md5[33]; // 32 characters + null terminator
    for (size_t i = 0; i < users_data_set.size; ++i) {
	    for (std::string word : dictionary) {
            bytes2md5(word.c_str(), word.length(), md5);

            if (strcmp(md5, users_data_set.data[i].hashed_password.c_str()) == 0) {
                printf("%s\t%s\t%s\n", users_data_set.data[i].mail.c_str(), word.c_str(), md5);
            }
        }
    }
}



static size_t load_data_from_file(std::string path, std::shared_ptr<DataToHack[]> &ptr_to_data_storage) {
    CSV_File users_data(path, "\t");
    size_t data_size = users_data.get_rows_number();

    std::vector<std::string> csv_mails, csv_hashed_passwords;
    csv_mails = users_data.get_column_by_number(2);
    csv_hashed_passwords = users_data.get_column_by_number(1);
    for (auto &dupa: csv_hashed_passwords) {
        dupa.pop_back();
    }

    ptr_to_data_storage = std::shared_ptr<DataToHack[]>(new DataToHack[data_size]);

    for (size_t i = 0; i < data_size; ++i) {
        ptr_to_data_storage[i].mail = csv_mails[i];
        ptr_to_data_storage[i].hashed_password = csv_hashed_passwords[i];
    }

    return data_size;
}

static void load_dictionary_from_file(std::string path, std::list<std::string> &dict) {
    CSV_File dict_file(path, "\t");

    std::vector<std::string> words;
    words = dict_file.get_column_by_number(0);

    for (std::string word: words) {
        dict.push_back(word);
    }
}


int main() {
    std::cout << "Loading data form file" << std::endl;
    users_data_set.size = load_data_from_file("data.txt", users_data_set.data);
    std::cout << "Loading dictionary form file" << std::endl;
    load_dictionary_from_file("dict.txt", dictionary);

    for (size_t i = 0; i < users_data_set.size; ++i) {
        std::cout << users_data_set.data[i].mail << std::endl;
    }


    std::thread th1(thread_one);

    th1.join();

    return 0;
}