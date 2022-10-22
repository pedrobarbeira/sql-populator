//
// Created by pbarbeira on 22-10-2022.
//

#ifndef SQL_POPULATOR_CATEGORY_H
#define SQL_POPULATOR_CATEGORY_H

#include "utils.h"
#include <set>

class Category{
    std::string categoryID;
    std::string categoryName;
public:
    explicit Category(const std::string& categoryName){
        this->categoryID = utils::random_uuid();
        this->categoryName = categoryName;
    }
    std::string to_sql() const;
};

class Topic{
    std::string topicID;
    std::string topicName;
    bool accepted;
    std::set<Category*> categories;
public:
    explicit Topic(const std::string& topicName, std::set<Category*> categories){
        this->topicID = utils::random_uuid();
        this->topicName = topicName;
        this->accepted = utils::random_bool();
        this->categories = categories;
    }
    std::string to_sql() const;
    bool contains(Category* c){
        return categories.contains(c);
    }
};

#endif //SQL_POPULATOR_CATEGORY_H
