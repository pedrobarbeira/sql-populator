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
    template<typename T, typename K>
    class mockdata {
        std::ifstream file;
        std::map<T, K> data;
        std::set<std::string> nameMap;

        void load_name_data();
        void load_surname_data();
        void load_publisher_data(const std::vector<Publisher*>& pubs);
        void load_sentence_data();
        std::vector<Topic*> generate_user_topics() const;
        std::vector<Topic*> generate_proposed_topics() const;
        std::vector<Category*> generate_article_categories() const;
        std::vector<Topic*> generate_article_topics(const std::vector<Category*>& articleCategories) const;
        std::vector<Comment*> generate_article_comments(const std::vector<Publisher*>& pubs) const;
        std::vector<Comment*> generate_comment_answers(const std::vector<Publisher*>& pubs) const;
    public:
        Publisher* generate_user();
        Friendship* generate_friendship(Publisher* pub, const std::vector<Publisher*>& pubs);
        Article* generate_article(const std::vector<Publisher*>& pubs);
        std::vector<Message*> generate_message(Friendship* f);
        void clear_name_data();
    };
    std::vector<Publisher*> generate_users(uint32_t n);
    std::vector<Friendship*>generate_relationships(const std::vector<Publisher*>& pubs);
    std::vector<Article*> generate_articles(uint32_t n, const std::vector<Publisher*>& pubs);
    std::vector<Message*> generate_messages(const std::vector<Friendship*>& friends);
    std::vector<UserReport*> generate_reports(const std::vector<Post*>& posts, const bool& article, const std::vector<Publisher*>& pubs);
    std::vector<Favorite*> generate_favorites(const std::vector<Article*>& articles, const std::vector<Publisher*>& pubs);
    void generate_mock_data();
    void sqlizer();
}


#endif //SQL_POPULATOR_MOCKDATA_H
