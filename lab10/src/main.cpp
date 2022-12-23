#include <iostream>
#include <thread>
#include <list>
#include <signal.h>
#include <openssl/evp.h>
#include <list>
#include <condition_variable>
#include <mutex>
#include <string.h>
#include "csv_file.hh"
#include "user.hh"

std::mutex passwords_mutex;
std::condition_variable password_cv;
std::list<UserHackedData> hacked_users;
std::list<std::string> dictionary;
DataToHackCollection users_data_set;
std::atomic_bool thread_one_running;
std::atomic_bool thread_two_running;

// static void sighub_handler(int sig) {
//     printf("Hack summary: %d\n", sig);
//     exit(0);
// }

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

static void add_hacked_password_to_global(DataToHack *user_data, std::string hacked_password) {
    UserHackedData data(
        user_data->mail,
        user_data->hashed_password,
        hacked_password
    );

    std::unique_lock<std::mutex> lock(passwords_mutex);
    hacked_users.push_front(data);
    password_cv.notify_all();
    lock.unlock();
}

void thread_one(void) {
    char md5[33]; // 32 characters + null terminator
    thread_one_running = true;
    for (size_t i = 0; i < users_data_set.size; ++i) {
	    for (std::string word : dictionary) {
            bytes2md5(word.c_str(), word.length(), md5);

            if (strcmp(md5, users_data_set.data[i].hashed_password.c_str()) == 0) {
                printf("%s\t%s\t%s\n", users_data_set.data[i].mail.c_str(), word.c_str(), md5);
                add_hacked_password_to_global(&users_data_set.data[i], word);
            }
        }
    }

    thread_one_running = false;
}

bool check_word(std::string word) {
    
}


void thread_two_hacking(std::string word) {
    std::string extended_word;
    for (int i = 0; i < 100; ++i) {
        extended_word = word + std::to_string(i);
    }
}

void thread_two(void) {
    char md5[33]; // 32 characters + null terminator
    thread_two_running = true;
    for (size_t i = 0; i < users_data_set.size; ++i) {
	    for (std::string word : dictionary) {
            for (int j = 0; j < 10; ++j) {
                std::string new_word = char(j + 48) + word;
                // std::cout << new_word << std::endl;
                bytes2md5(new_word.c_str(), new_word.length(), md5);

                if (strcmp(md5, users_data_set.data[i].hashed_password.c_str()) == 0) {
                    printf("%s\t%s\t%s\n", users_data_set.data[i].mail.c_str(), new_word.c_str(), md5);
                    add_hacked_password_to_global(&users_data_set.data[i], new_word);
                }
            }
        }
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    thread_two_running = false;
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

    std::thread th1(thread_one);
    std::thread th2(thread_two);

    while (true) {
        // auto now = std::chrono::system_clock::now();
        std::cout << "wait" << std::endl;
        // if (password_cv.wait_until(lck, now + std::chrono::milliseconds(500)) == std::cv_status::no_timeout) {
        //     UserHackedData new_data = hacked_users.front();
        //     std::cout << new_data.hashed_password << " ======> " << new_data.hacked_password << std::endl;
        // }
        std::unique_lock<std::mutex> lck(passwords_mutex);
        password_cv.wait(lck);
            UserHackedData new_data = hacked_users.front();
            std::cout << new_data.hashed_password << " ======> " << new_data.hacked_password << std::endl;

    }
    th1.join();
    th2.join();

    return 0;
}