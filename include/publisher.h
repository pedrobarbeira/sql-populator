#ifndef SQL_POPULATOR_PUBLISHER_H
#define SQL_POPULATOR_PUBLISHER_H

#include <iostream>
#include <cstdio>
#include "configs.h"
#include "utils.h"
#include "category.h"

static uint32_t publisherIdCount = 0;
static uint32_t bannedUsers = MAX_BANNED_USERS;

class Publisher{
    uint32_t userID;
    std::string name;
    std::string password;
    std::string email;
    std::string profilePic;
    std::string creationDate;
    uint32_t nFriends;
    uint32_t posts;
    std::string bio;
    uint32_t reputation;
    std::string banned;
    std::vector<Topic*> feedTopic;
    std::vector<Topic*> proposedTopics;
public:
    Publisher(const std::string& username, const std::vector<Topic*>& feedTopic, const std::vector<Topic*>& proposedTopics){
        this->userID = ++publisherIdCount;
        this->name = std::move(utils::stringify(username));
        unsigned char* md;
        unsigned char* cypherPassword = SHA256(reinterpret_cast<const unsigned char *>(username.c_str()), sizeof(char) * username.size(), md);
        std::string cyphered(reinterpret_cast<char*>(md));
        this->password = std::move(utils::stringify(cyphered));
        std::string mailHead = username;
        utils::all_lower(mailHead);
        this->profilePic = utils::stringify("");
        this->email = std::move(utils::stringify(mailHead + "@gmail.com"));
        this->creationDate = std::move(utils::stringify(utils::random_date()));
        this->nFriends = 0;
        this->posts = 0;
        this->bio = std::move(utils::stringify("Hi, my name is " + username));
        this->reputation = 0;
        if(bannedUsers > 0){
            if(utils::random_bool()){
                this->banned = utils::stringify("TRUE");
                bannedUsers--;
            }
        }
        this ->banned = utils::stringify("FALSE");
        this->feedTopic = feedTopic;
        this->proposedTopics = proposedTopics;
    }

    void to_sql(){
            FILE *file = fopen(USER_OUTPUT, "a");
            fprintf(file, PUBLISHER_TEMPLATE, this->userID, this->name.c_str(), this->email.c_str(), this->password.c_str(),
                    this->creationDate.c_str(),  this->nFriends,this->profilePic.c_str(), this->bio.c_str(), this->posts,
                    this->reputation, this->banned.c_str());
            if(this->banned == "TRUE"){
                fprintf(file, BAN_TEMPLATE, this->userID, 1);
            }
            for(auto i : feedTopic){
                fprintf(file, FEED_TOPIC_TEMPLATE, this->userID, i->get_id());
            }
            for(auto i : proposedTopics){
                fprintf(file, PROPOSE_TOPIC_TEMPLATE, i->get_id(), this->userID);
            }
            fclose(file);
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

    void add_post(){
        this->posts++;
    }

    void set_reputation(uint32_t reputation){
        this->reputation = reputation;
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
    void to_sql(){
        FILE* f = fopen(FAVORITE_OUTPUT, "a");
        fprintf(f, FAVORITES_TEMPLATE, this->publisherID, this->articleID);
        fclose(f);
    }
};


#endif //SQL_POPULATOR_PUBLISHER_H
