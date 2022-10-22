#ifndef SQL_POPULATOR_PUBLISHER_H
#define SQL_POPULATOR_PUBLISHER_H

#include <iostream>
#include <cstdio>
#include "configs.h"
#include "utils.h"
#include "object.h"
#include "category.h"

class Publisher : public Object{
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
    std::vector<Topic*> feedTopic;
    std::vector<Topic*> proposedTopics;
public:
    Publisher(const std::string& username, const std::vector<Topic*>& feedTopic, const std::vector<Topic*>& proposedTopics);
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

class Favorite{
    std::string publisherID;
    std::string articleID;
public:
    Favorite(const std::string& publisherID, const std::string& articleID){
        this->publisherID = publisherID;
        this->articleID = articleID;
    }
    std::string to_sql();
};


#endif //SQL_POPULATOR_PUBLISHER_H
