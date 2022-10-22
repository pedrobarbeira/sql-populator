//
// Created by pbarbeira on 20-10-2022.
//

#ifndef SQL_POPULATOR_MESSAGE_H
#define SQL_POPULATOR_MESSAGE_H

#include "utils.h"

static uint32_t messageIdCount = 0;

class Message{
    uint32_t messageID;
    std::string content;
    std::string date;
    Publisher* sender;
    Publisher* receiver;
public:
    explicit Message(Publisher* sender, Publisher* receiver, const std::string& content){
        this->messageID = ++messageIdCount;
        this->content = content;
        this->date = utils::random_date();
        this->sender = sender;
        this->receiver = receiver;
    }
    std::string to_sql() const;
};

#endif //SQL_POPULATOR_MESSAGE_H
