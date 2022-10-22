//
// Created by pbarbeira on 20-10-2022.
//

#ifndef SQL_POPULATOR_MESSAGE_H
#define SQL_POPULATOR_MESSAGE_H

#include "utils.h"

class Message{
    std::string messageID;
    std::string content;
    std::string date;
public:
    explicit Message(const std::string& content){
        this->messageID = utils::random_uuid();
        this->content = content;
        this->date = utils::random_date();
    }
    std::string to_sql() const;
};

#endif //SQL_POPULATOR_MESSAGE_H
