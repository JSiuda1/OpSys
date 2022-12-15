#pragma once

#include <string>
#include <atomic>

struct DataToHack {
    std::string mail;
    std::string hashed_password;
    std::atomic_bool hacked;
    DataToHack() : hacked(false) {};
    DataToHack(std::string _mail, std::string _passwd):
        mail(_mail),
        hashed_password(_passwd),
        hacked(false)
    {};
};

struct UserHackedData {
    std::string mail;
    std::string hashed_password;
    std::string hacked_password;
    UserHackedData(std::string _mail, std::string hashed_paasswd, std::string hacked_passwd):
        mail(_mail),
        hashed_password(hashed_paasswd),
        hacked_password(hacked_passwd)
    {};
};