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
#include "reports.h"

namespace mockdata {
    template<typename T>
    class mockdata {
        std::ifstream file;
        std::map<std::string, T> data;
        std::set<std::string> nameMap;

        void load_name_data();
        void load_surname_data();
        void load_publisher_data(const std::vector<Publisher*>& pubs);
        void load_sentence_data();
        std::vector<Topic*> generate_user_topics() const;
        std::vector<Topic*> generate_proposed_topics();
        std::vector<Category*> generate_article_categories() const;
        std::vector<Topic*> generate_article_topics(const std::vector<Category*>& articleCategories) const;
    public:
        Publisher* generate_user();
        Friendship* generate_friendship(Publisher& pub, const std::vector<Publisher*>& pubs);
        Article* generate_article(const std::vector<Publisher*>& pubs);
        Message* generate_message(Friendship* f);
        void clear_name_data();
        void clear_sentence_data();

        void driver();
    };
    std::vector<Publisher*> generate_users(uint32_t n);
    std::vector<Friendship*>generate_relationships(const std::vector<Publisher*>& pubs);
    std::vector<Article*> generate_articles(uint32_t n, const std::vector<Publisher*>& pubs);
    std::vector<Message*> generate_messages(const std::vector<Publisher*>& pubs);
    std::vector<UserReport*> generate_reports(const std::vector<Post*>& posts, const std::vector<Publisher*>& pubs);
    std::vector<Favorite*> generate_favorites(const std::vector<Article*>& articles, const std::vector<Publisher*>& pubs);
}


#endif //SQL_POPULATOR_MOCKDATA_H
