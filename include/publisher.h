#ifndef SQL_POPULATOR_PUBLISHER_H
#define SQL_POPULATOR_PUBLISHER_H

#include <iostream>
#include <cstdio>
#include "configs.h"
#include "utils.h"
#include "category.h"

static uint32_t publisherIdCount = 0;

class Publisher{
    uint32_t userID;
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
    std::vector<Topic*> feedTopic;
    std::vector<Topic*> proposedTopics;
public:
    Publisher(const std::string& username, const std::vector<Topic*>& feedTopic, const std::vector<Topic*>& proposedTopics){
        this->userID = ++publisherIdCount;
        this->name = std::move(utils::stringify(username));
        this->password = std::move(utils::stringify(username));
        std::string mailHead = username;
        utils::all_lower(mailHead);
        this->email = std::move(utils::stringify(mailHead + "@gmail.com"));
        this->creationDate = std::move(utils::stringify(utils::random_date()));
        this->deletionDate = std::move(utils::stringify("NULL"));
        this->nFriends = 0;
        this->posts = 0;
        this->bio = std::move(utils::stringify("Hi, my name is " + username));
        this->reputation = 0;
        this ->banned = false;
        this->feedTopic = feedTopic;
        this->proposedTopics = proposedTopics;
    }

    void to_sql(bool terminal){
        std::string boolean;
        if (this->banned) boolean = "\'True\'";
        else boolean = "\'False\'";
        if (terminal)
            printf(USER_TEMPLATE, this->userID, this->name.c_str(), this->password.c_str(), this->email.c_str(),
                   this->creationDate.c_str(), this->deletionDate.c_str(), this->nFriends, this->posts, this->bio.c_str(),
                   this->reputation, boolean.c_str());
        else {
            FILE *file = fopen(USER_OUTPUT, "a");
            fprintf(file, USER_TEMPLATE, this->userID, this->name.c_str(), this->password.c_str(), this->email.c_str(),
                    this->creationDate.c_str(), this->deletionDate.c_str(), this->nFriends, this->posts, this->bio.c_str(),
                    this->reputation, boolean.c_str());
        }
    }

    [[nodiscard]] uint32_t get_friends() const{
        return nFriends;
    }

    [[nodiscard]] uint32_t get_id() const{
        return userID;
    }

    void add_friend(){
        this->nFriends++;
    }
};

class Favorite{
    uint32_t publisherID;
    uint32_t articleID;
public:
    Favorite(const uint32_t& publisherID, const uint32_t& articleID){
        this->publisherID = publisherID;
        this->articleID = articleID;
    }
    std::string to_sql();
};


#endif //SQL_POPULATOR_PUBLISHER_H
