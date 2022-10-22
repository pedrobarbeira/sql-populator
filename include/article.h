//
// Created by pbarbeira on 20-10-2022.
//

#ifndef SQL_POPULATOR_ARTICLE_H
#define SQL_POPULATOR_ARTICLE_H

#include <iostream>
#include "publisher.h"

class Post : public Object{
protected:
    std::string postID;
    uint32_t nLikes;
    uint32_t nDislikes;
    uint32_t nComments;
    std::string postDate;
    Publisher* author;
public:
    explicit Post(uint32_t nComments, Publisher* author){
        this->postID = utils::random_uuid();
        this->nLikes = utils::random_uint32(MAX_USERS);
        this->nDislikes = utils::random_uint32(MAX_USERS);
        this->nComments = nComments;
        this->postDate = utils::random_date();
        this->author = author;
    }
};

class Article : public Post{
    std::string title;
    std::string description;
    std::string body;
    bool accepted;
    std::vector<Category*> categories;
    std::vector<Topic*> topics;
public:
    Article(const std::string& title, const std::string& description, const std::string& body, Publisher* author,
            const std::vector<Category*>& categories, const std::vector<Topic*>& topics) :
            Post(utils::random_uint32(20), author){
        this->title = title;
        this->description = description;
        this->body =body;
        this->accepted = utils::random_bool();
        this->categories = categories;
        this->topics = topics;
    }
    std::string to_sql() const;
    bool validTopic(Topic* t){
        for(auto i : categories){
            if(t->contains(i)) {
                return true;
            }
        }
        return false;
    }
};

class Comment : public Post{
    std::string content;
public:
    Comment(const std::string& content, Publisher* author) :
        Post(utils::random_uint32(5), author){
        this->content = content;
    }
    std::string to_sql(const std::string& articleUuid) const;
};

#endif //SQL_POPULATOR_ARTICLE_H
