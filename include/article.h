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
        this->postDate = utils::stringify(utils::random_date());
        this->author = author;
    }

    [[nodiscard]] Publisher* get_author() const{
        return author;
    }

    uint32_t get_id(){
        return this->postID;
    }
};

class Comment : public Post{
    std::string content;
    std::vector<Comment*> answers;
public:
    Comment(const std::string& content, Publisher* author, const std::vector<Comment*>& answers) :
            Post(++commentIdCount, utils::random_uint32(5), author){
        this->content = std::string(content);
        this->answers = answers;
    }
    void to_sql(const uint32_t & articleUuid) const{
        FILE* f = fopen(COMMENT_OUTPUT, "a");
        fprintf(f, COMMENT_TEMPLATE, this->postID, this->content.c_str(), this->nLikes, this->nDislikes,
                this->postDate.c_str());
        fprintf(f, COMMENT_ARTICLE_TEMPLATE, this->postID, articleUuid);
        fprintf(f, WRITE_COMMENT_TEMPLATE, this->postID, this->author->get_id());
        for(auto i : answers){
            fprintf(f, ANSWER_TEMPLATE, this->postID, i->get_id());
            fprintf(f, COMMENT_ARTICLE_TEMPLATE, i->postID, articleUuid);
        }
        fclose(f);
    }

    uint32_t get_id() const{
        return this->postID;
    }
};

class Article : public Post{
    std::string title;
    std::string description;
    std::string body;
    std::string accepted;
    std::vector<Category*> categories;
    std::vector<Topic*> topics;
    std::vector<Comment*> comments;
public:
    Article(const std::string& title, const std::string& description, const std::string& body, Publisher* author,
            const std::vector<Category*>& categories, const std::vector<Topic*>& topics, const std::vector<Comment*>& comments) :
            Post(++articleIdCount, utils::random_uint32(20), author){
        this->title = utils::stringify(title);
        this->description = utils::stringify(description);
        this->body = utils::stringify(body);
        if(utils::random_bool()){
            this->accepted = utils::stringify("TRUE");
        }
        else{
            this->accepted = utils::stringify("FALSE");
        }
        this->categories = categories;
        this->topics = topics;
        this->comments = comments;
    }
    void to_sql() const{
        FILE* f = fopen(ARTICLE_OUTPUT, "a");
        fprintf(f, ARTICLE_TEMPLATE, this->postID, this->title.c_str(), this->description.c_str(), this->body.c_str(),
                this->accepted.c_str(), this->nLikes, this->nDislikes, this->nComments, this->postDate.c_str());
        for(auto i : categories){
            fprintf(f, ARTICLE_CATEGORY_TEMPLATE, this->postID, i->get_id());
        }
        for(auto i : topics){
            fprintf(f, ARTICLE_HAS_TOPIC_TEMPLATE, this->postID, i->get_id());
        }
        for(auto i : comments){
            i->to_sql(this->postID);
        }
        fprintf(f, WRITE_ARTICLE_TEMPLATE, this->postID, this->author->get_id());
        fclose(f);
    }

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

#endif //SQL_POPULATOR_ARTICLE_H
