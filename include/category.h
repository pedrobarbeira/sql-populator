//
// Created by pbarbeira on 22-10-2022.
//

#ifndef SQL_POPULATOR_CATEGORY_H
#define SQL_POPULATOR_CATEGORY_H

#include "utils.h"
#include <set>

class Category;
class Topic;

static uint32_t categoryIdCount = 0;
static uint32_t topicIdCount = 0;

static std::vector<Category*> globalCategories;
static std::vector<Topic*> globalTopics;

class Category{
    uint32_t categoryID;
    std::string categoryName;
public:
    explicit Category(const std::string& categoryName){
        this->categoryID = ++categoryIdCount;
        this->categoryName = utils::stringify(categoryName);
    }
    void to_sql() const{
        FILE* f = fopen(CATEGORIES_OUTPUT, "a");
        fprintf(f, CATEGORY_TEMPLATE, this->categoryID, this->categoryName.c_str());
        fclose(f);
    }

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

    [[nodiscard]] uint32_t get_id() const{
        return this->categoryID;
    }
};


class Topic{
    uint32_t topicID;
    std::string topicName;
    std::string accepted;
    std::set<Category*> categories;
public:
    explicit Topic(const std::string& topicName, const std::set<Category*>& categories){
        this->topicID = ++topicIdCount;
        this->topicName = utils::stringify(topicName);
        if(utils::random_bool()){
            this->accepted = utils::stringify("TRUE");
        }
        else{
            this->accepted = utils::stringify("FALSE");
        }
        this->categories = categories;
    }

    void to_sql(){
        FILE* f = fopen(TOPICS_OUTPUT, "a");
        fprintf(f, TOPIC_TEMPLATE, this->topicID, this->topicName.c_str(), this->accepted.c_str());
        for (auto i : this->categories){
            fprintf(f, CATEGORY_TOPIC_TEMPLATE, this->topicID, i->get_id());
        }
        fclose(f);
    }

    bool contains(Category* c){
        return categories.contains(c);
    }

    static std::vector<Topic*> get_topics(){
        if(globalTopics.empty()){
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
                for(j = 0; j < utils::random_uint32(globalCategories.size() - 1); j++){
                    uint32_t x = utils::random_uint32(globalCategories.size() - 1);
                    topicCategories.insert(globalCategories[x]);
                }
                auto topic = new Topic(words[k], topicCategories);
                globalTopics.push_back(topic);
            }
        }
        return globalTopics;
    }

    [[nodiscard]] bool isAccepted() const{
        return this->accepted == utils::stringify("TRUE");
    }

    uint32_t get_id() const{
        return this->topicID;
    }
};

#endif //SQL_POPULATOR_CATEGORY_H
