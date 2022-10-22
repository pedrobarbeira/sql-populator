#ifndef SQL_POPULATOR_PUBLISHER_H
#define SQL_POPULATOR_PUBLISHER_H

#include <iostream>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>
#include <openssl/sha.h>
#include <openssl/evp.h>
#include <cstdio>
#include <random>
#include "configs.h"

uint32_t random_number(int max);
std::string stringify(std::string str);

class Publisher {
    std::string userID;
    std::string name;
    std::string password;
    std::string email;
    std::string creationDate;
    std::string deletionDate;
    uint32_t nFriends;
    uint32_t posts;
    std::string bio;
    int reputation;
    bool banned;
public:
    explicit Publisher(const std::string& username);
    void to_sql(bool terminal);

    [[nodiscard]] uint32_t get_friends() const{
        return nFriends;
    }

    [[nodiscard]] std::string get_id() const{
        return userID;
    }

    void add_friend(){
        this->nFriends++;
    }
};


#endif //SQL_POPULATOR_PUBLISHER_H
