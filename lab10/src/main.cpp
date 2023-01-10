#include <iostream>
#include <thread>
#include <list>
#include <signal.h>
#include <openssl/evp.h>
#include <list>
#include <condition_variable>
#include <algorithm>
#include <mutex>
#include <string.h>
#include "csv_file.hh"
#include "user.hh"

std::mutex passwords_mutex;
std::condition_variable password_cv;
std::list<UserHackedData> hacked_users;
std::list<std::string> dictionary;
DataToHackCollection users_data_set;
const int task_count = 7;
std::atomic_int finished_tasks_counter;

static void sighub_handler(int sig) {
    std::cout << "=============================="<< std::endl;
    std::cout << "Hack summary:" << std::endl;
    std::unique_lock<std::mutex> lock(passwords_mutex);
    for (UserHackedData i : hacked_users) {
        std::cout << i.mail << " ==== > " << i.hacked_password << std::endl;
    }
    std::cout << "=============================="<< std::endl;
}


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
}

void thread_one(void) {
    char md5[33]; // 32 characters + null terminator
    for (size_t i = 0; i < users_data_set.size; ++i) {
	    for (std::string word : dictionary) {
            bytes2md5(word.c_str(), word.length(), md5);

            if (strcmp(md5, users_data_set.data[i].hashed_password.c_str()) == 0) {
                add_hacked_password_to_global(&users_data_set.data[i], word);
            }
        }
    }

    finished_tasks_counter += 1;
}

bool check_word(std::string word, std::string hashed_pswd) {
    char md5[33];
    bytes2md5(word.c_str(), word.length(), md5);

    if (strcmp(md5, hashed_pswd.c_str()) == 0) {
        return true;
    }

    return false;
}

static void check_word_over_data_set(std::string word, DataToHackCollection * data) {
    for (size_t j= 0; j < data->size; ++j) {
            if (check_word(word, data->data[j].hashed_password) == true) {
                if (data->data[j].hacked == false) {
                    add_hacked_password_to_global(&data->data[j], word);
                    data->data[j].hacked = true;
                }
            }
        }
}

void check_word_with_preposfix(std::string word, DataToHackCollection *data) {
    std::string extended_word;
    for (int i = 0; i < 100; ++i) {
        if (i < 10) {
            extended_word = word + std::to_string(0) + std::to_string(i);
        } else {
            extended_word = word + std::to_string(i);
        }
        check_word_over_data_set(extended_word, data);

        std::string double_extended_word;
        for (int j = 0; j < 100; ++j) {
            if (j < 10) {
                double_extended_word = std::to_string(0) + std::to_string(j) + extended_word;
            } else {
                double_extended_word = std::to_string(j) + extended_word;
            }
            check_word_over_data_set(double_extended_word, data);
        }
    }

    for (int i = 0; i < 10; ++i) {
        extended_word = std::to_string(i) + word;
        check_word_over_data_set(extended_word, data);
        extended_word = word + std::to_string(i);
        check_word_over_data_set(extended_word, data);
    }
}

void thread_two(void) {
    for (std::string word : dictionary) {
        check_word_with_preposfix(word, &users_data_set);
    }

    finished_tasks_counter += 1;
}

void thread_all_upper(void) {
    for (std::string word : dictionary) {
        std::transform(word.begin(), word.end()+1, word.begin(), ::toupper);
        check_word_over_data_set(word, &users_data_set);
    }

    finished_tasks_counter += 1;
}

void thread_all_upper_postprefix(void) {
    for (std::string word : dictionary) {
        std::transform(word.begin(), word.end()+1, word.begin(), ::toupper);
        check_word_with_preposfix(word, &users_data_set);
    }

    finished_tasks_counter += 1;
}

void thread_first_upper(void) {
    for (std::string word : dictionary) {
        word[0] = toupper(word[0]);
        check_word_over_data_set(word, &users_data_set);
    }

    finished_tasks_counter += 1;
}

void thread_first_upper_postprefix(void) {
    for (std::string word : dictionary) {
        word[0] = toupper(word[0]);
        check_word_with_preposfix(word, &users_data_set);
    }

    finished_tasks_counter += 1;
}

void thread_all_lower(void) {
    for (std::string word : dictionary) {
        check_word_over_data_set(word, &users_data_set);
    }

    finished_tasks_counter += 1;
}

void thread_all_lower_postprefix(void) {
    for (std::string word : dictionary) {
        check_word_with_preposfix(word, &users_data_set);
    }

    finished_tasks_counter += 1;
}

void thread_two_words(void) {
    for (std::string first_word : dictionary) {
        for (std::string second_word : dictionary) {
            std::string two_word = first_word + " " + second_word;
            check_word_over_data_set(two_word, &users_data_set);
        }
    }

    finished_tasks_counter += 1;
}

void consumer_thread(void) {
    while (finished_tasks_counter < task_count) {
        auto now = std::chrono::system_clock::now();
        std::unique_lock<std::mutex> lck(passwords_mutex);
        if (password_cv.wait_until(lck, now + std::chrono::milliseconds(100)) == std::cv_status::no_timeout) {
            UserHackedData new_data = hacked_users.front();
            std::cout << new_data.hashed_password << " ======> " << new_data.hacked_password << std::endl;
        }
    }
}

static size_t load_data_from_file(std::string path, std::shared_ptr<DataToHack[]> &ptr_to_data_storage) {
    CSV_File users_data(path, "\t");
    size_t data_size = users_data.get_rows_number();

    std::vector<std::string> csv_mails, csv_hashed_passwords;
    csv_mails = users_data.get_column_by_number(2);
    csv_hashed_passwords = users_data.get_column_by_number(1);
    for (auto &data: csv_hashed_passwords) {
        data.pop_back();
    }

    ptr_to_data_storage = std::shared_ptr<DataToHack[]>(new DataToHack[data_size]);

    for (size_t i = 0; i < data_size; ++i) {
        ptr_to_data_storage[i].mail = csv_mails[i];
        ptr_to_data_storage[i].hashed_password = csv_hashed_passwords[i];
        std::cout << csv_mails[i] << "\t" << csv_hashed_passwords[i] << std::endl;
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
    signal(SIGHUP, sighub_handler);
    std::cout << "Loading data form file" << std::endl;
    users_data_set.size = load_data_from_file("data_mini.txt", users_data_set.data);
    std::cout << "Data loaded" << std::endl;
    std::cout << "Loading dictionary form file" << std::endl;
    load_dictionary_from_file("dict.txt", dictionary);
    std::cout << "Dictionary loaded" << std::endl;

    std::thread th_all_upper(thread_all_upper);
    std::thread th_all_upper_fix(thread_all_upper_postprefix);
    std::thread th_all_lower(thread_all_lower);
    std::thread th_all_lower_fix(thread_all_lower_postprefix);
    std::thread th_first_upper(thread_first_upper);
    std::thread th_first_upper_fix(thread_first_upper_postprefix);
    std::thread th_two_words(thread_two_words);
    std::thread th_consumer(consumer_thread);

    th_all_upper.join();
    th_all_upper_fix.join();
    th_all_lower.join();
    th_all_lower_fix.join();
    th_first_upper.join();
    th_first_upper_fix.join();
    th_two_words.join();
    th_consumer.join();

    return 0;
}