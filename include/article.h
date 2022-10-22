//
// Created by pbarbeira on 20-10-2022.
//

#ifndef SQL_POPULATOR_ARTICLE_H
#define SQL_POPULATOR_ARTICLE_H

#include <iostream>
#include "publisher.h"

static uint32_t articleIdCount = 0;
static uint32_t commentIdCount = 0;

class Comment;

class Post {
protected:
    uint32_t postID;
    uint32_t nLikes;
    uint32_t nDislikes;
    uint32_t nComments;
    std::string postDate;
    Publisher* author;
public:
    explicit Post(uint32_t id, uint32_t nComments, Publisher* author){
        this->postID = id;
        this->nLikes = utils::random_uint32(MAX_USERS);
        this->nDislikes = utils::random_uint32(MAX_USERS);
        this->nComments = nComments;
        this->postDate = utils::random_date();
        this->author = author;
    }

    [[nodiscard]] Publisher* get_author() const{
        return author;
    }
};

class Article : public Post{
    std::string title;
    std::string description;
    std::string body;
    bool accepted;
    std::vector<Category*> categories;
    std::vector<Topic*> topics;
    std::vector<Comment*> comments;
public:
    Article(const std::string& title, const std::string& description, const std::string& body, Publisher* author,
            const std::vector<Category*>& categories, const std::vector<Topic*>& topics, const std::vector<Comment*>& comments) :
            Post(++articleIdCount, utils::random_uint32(20), author){
        this->title = title;
        this->description = description;
        this->body =body;
        this->accepted = utils::random_bool();
        this->categories = categories;
        this->topics = topics;
        this->comments = comments;
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

    [[nodiscard]] uint32_t get_id() const{
        return this->postID;
    }
};

class Comment : public Post{
    std::string content;
    std::vector<Comment*> answers;
public:
    Comment(const std::string& content, Publisher* author, const std::vector<Comment*>& answers) :
        Post(++commentIdCount, utils::random_uint32(5), author){
        this->content = content;
        this->answers = answers;
    }
    std::string to_sql(const std::string& articleUuid) const;
};

#endif //SQL_POPULATOR_ARTICLE_H
