#include "../include/publisher.h"

Publisher::Publisher(Publisher(const std::string& username, const std::vector<Topic*>& feedTopic, const std::vector<Topic*>& proposedTopics){
    this->userID = utils::random_uuid();
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
void Publisher::to_sql(bool terminal) {
    std::string boolean;
    if (this->banned) boolean = "\'True\'";
    else boolean = "\'False\'";
    if (terminal)
        printf(USER_TEMPLATE, this->userID.c_str(), this->name.c_str(), this->password.c_str(), this->email.c_str(),
               this->creationDate.c_str(), this->deletionDate.c_str(), this->nFriends, this->posts, this->bio.c_str(),
               this->reputation, boolean.c_str());
    else {
        FILE *file = fopen(USER_OUTPUT, "a");
        fprintf(file, USER_TEMPLATE, this->userID.c_str(), this->name.c_str(), this->password.c_str(), this->email.c_str(),
                this->creationDate.c_str(), this->deletionDate.c_str(), this->nFriends, this->posts, this->bio.c_str(),
                this->reputation, boolean.c_str());
    }
}
