#include <iostream>
#include <thread>
#include <list>
#include <signal.h>
#include <openssl/evp.h>
#include <memory>
#include "csv_file.hh"
#include "user.hh"

// static void sighub_handler(int sig) {
//     printf("Hack summary: %d\n", sig);
//     exit(0);
// }

// static void import_dictionary() {
//     FILE *file;
//     char dict_file_name[] = "dict.txt";

//     // printf("Insert dictionary file\n");
//     // scanf("%[^\n]s", dict_file_name);
//     file = fopen(dict_file_name, "r");
//     if (file == NULL) {
//         printf("Can't find file with name %s\n", dict_file_name);
//         exit(1);
//     }

//     fclose(file);
//     printf("Dictionary imported");
// }

// static void import_passwords() {
//     FILE *file;
//     char password_file_name[100];

//     printf("Insert passowrd file\n");
//     scanf("%[^\n]s", password_file_name);

//     file = fopen(password_file_name, "r");
//     if (file == NULL) {
//         printf("Can't find file with name %s\n", password_file_name);
//         exit(1);
//     }

//     fclose(file);
//     printf("Dictionary imported");
// }

// static void bytes2md5(const char *data, int len, char *md5buf) {
// 	EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
// 	const EVP_MD *md = EVP_md5();
// 	unsigned char md_value[EVP_MAX_MD_SIZE];
// 	unsigned int md_len;
// 	EVP_DigestInit_ex(mdctx, md, NULL);
// 	EVP_DigestUpdate(mdctx, data, len);
// 	EVP_DigestFinal_ex(mdctx, md_value, &md_len);
// 	EVP_MD_CTX_free(mdctx);
// 	for (size_t i = 0; i < md_len; i++) {
// 		snprintf(&(md5buf[i * 2]), 16 * 2, "%02x", md_value[i]);
// 	}
// }

// void first_hacker(void) {
//     const char *test = "Help";
// 	char md5[33]; // 32 characters + null terminator
// 	bytes2md5(test, strlen(test), md5);
// 	printf("%s ======================> %s\n", test, md5);
// }

static size_t load_data_from_file(std::string path, std::shared_ptr<DataToHack[]> &ptr_to_data_storage) {
    CSV_File users_data(path, "\t");
    size_t data_size = users_data.get_rows_number();

    std::vector<std::string> csv_mails, csv_hashed_passwords;
    csv_mails = users_data.get_column_by_number(2);
    csv_hashed_passwords = users_data.get_column_by_number(3);

    ptr_to_data_storage = std::shared_ptr<DataToHack[]>(new DataToHack[data_size]);

    for (size_t i = 0; i < data_size; ++i) {
        ptr_to_data_storage[i].mail = csv_mails[i];
        ptr_to_data_storage[i].hashed_password = csv_hashed_passwords[i];
    }

    return data_size;
}


int main() {
    std::shared_ptr<DataToHack[]> test;
    size_t data_size = load_data_from_file("dict.txt", test);

    for(size_t i =0 ; i < data_size; ++i) {
        std::cout << test[i].mail << "\t" << test[i].hashed_password << std::endl;
    }

    return 0;
}