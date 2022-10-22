//
// Created by pbarbeira on 22-10-2022.
//

#ifndef SQL_POPULATOR_CATEGORY_H
#define SQL_POPULATOR_CATEGORY_H

#include "utils.h"
#include <set>

class Category;
class Topic;

std::vector<Category*> globalCategories;
std::vector<Topic*> topics;

class Category{
    std::string categoryID;
    std::string categoryName;
public:
    explicit Category(const std::string& categoryName){
        this->categoryID = utils::random_uuid();
        this->categoryName = categoryName;
    }
    std::string to_sql() const;
    static std::vector<Category*> get_categories(){
        if(globalCategories.empty()) {
            std::cout << "[LOG] Loading categories\n";
            std::ifstream file(CATEGORIES_FILE);
            if (file.fail()) {
                throw FileNotFoundException(CATEGORIES_FILE);
            }
            std::string line;
            while (!file.eof()) {
                getline(file, line);
                auto *category = new Category(line);
                globalCategories.push_back(category);
            }
            file.close();
        }
        return globalCategories;
    }
};


class Topic{
    std::string topicID;
    std::string topicName;
    bool accepted;
    std::set<Category*> categories;
public:
    explicit Topic(const std::string& topicName, const std::set<Category*>& categories){
        this->topicID = utils::random_uuid();
        this->topicName = topicName;
        this->accepted = utils::random_bool();
        this->categories = categories;
    }
    std::string to_sql() const;
    bool contains(Category* c){
        return categories.contains(c);
    }
    static std::vector<Topic*> get_topics(){
        if(topics.empty()){
            if(globalCategories.empty()){
                try {
                    Category::get_categories();
                }catch(const FileNotFoundException& e){
                    std::cout << e.what();
                }
            }
            std::ifstream file(TOPICS_FILE);
            if (file.fail()) {
                throw FileNotFoundException(TOPICS_FILE);
            }
            std::string line;
            std::vector<std::string> words;
            while (!file.eof()) {
                getline(file, line);
                words.push_back(line);
            }
            uint32_t j, k;
            for(int i = 0; i < MAX_TOPICS; i++){
                k = utils::random_uint32(words.size());
                std::set<Category*> topicCategories;
                for(j = 0; j < utils::random_uint32(globalCategories.size()); j++){
                    uint32_t x = utils::random_uint32(globalCategories.size());
                    topicCategories.insert(globalCategories[x]);
                }
                auto topic = new Topic(words[k], topicCategories);
                topics.push_back(topic);
            }
        }
        return topics;
    }
};

#endif //SQL_POPULATOR_CATEGORY_H
