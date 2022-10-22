#ifndef SQL_POPULATOR_MOCKDATA_H
#define SQL_POPULATOR_MOCKDATA_H

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include "exceptions.h"
#include <random>
#include "publisher.h"
#include "friendship.h"
#include "article.h"
#include "message.h"
#include <set>
#include "utils.h"

namespace mockdata {
    template<typename T>
    class mockdata {
        std::ifstream file;
        std::map<std::string, T> data;

        void load_name_data();
        void load_surname_data();
        void load_publisher_data(const std::vector<Publisher*>& pubs);
        void load_sentence_data();
    public:
        std::pair<std::string, std::string> generate_user();
        Friendship* generate_friendship(Publisher& pub, const std::vector<Publisher*>& pubs);
        Message* generate_message(Friendship* f);
        Article* generate_article();
        void clear_name_data();
        void clear_friendship_data();
        void clear_sentence_data();

        void driver();
    };
    std::vector<std::string> generate_users(uint32_t n);
    std::vector<Friendship*>generate_relationships(const std::vector<Publisher*>& pubs);
    std::vector<Article*> generate_articles(uint32_t n);

    void driver();
}


#endif //SQL_POPULATOR_MOCKDATA_H
