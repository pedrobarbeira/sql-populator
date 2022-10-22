#include "../include/publisher.h"

uint32_t random_number(int max){
    std::random_device rd;
    std::uniform_int_distribution<int> dist(1, max);
    return dist(rd);
}

std::string stringify(std::string str){
    return "\'" + str + "\'";
}

std::string random_date(){
    uint32_t year = 2020 + random_number(2);
    uint32_t month = 1 + random_number(11);
    uint32_t day;
    if(month == 2) day = 1 + random_number(27);
    else if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)
        day = 1 + random_number(30);
    else day = 1 + random_number(29);
    std::stringstream ss;
    ss << year << '-' << month << '-' << day;
    return ss.str();
}

void all_lower(std::string& str){
    for(int i = 0; i < str.size(); i++){
        if(isupper(str[i])) str[i] = tolower(str[i]);
    }
}

Publisher::Publisher(const std::string& username){
    boost::uuids::uuid uuid = boost::uuids::random_generator()();
    this->userID = boost::lexical_cast<std::string>(uuid);
    this->name = std::move(stringify(username));
    this->password = std::move(stringify(username));
    std::string mailHead = username;
    all_lower(mailHead);
    this->email = std::move(stringify(mailHead + "@gmail.com"));
    this->creationDate = std::move(stringify(random_date()));
    this->deletionDate = std::move(stringify("NULL"));
    this->nFriends = 0;
    this->posts = 0;
    this->bio = std::move(stringify("Hi, my name is " + username));
    this->reputation = 0;
    this ->banned = false;
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
