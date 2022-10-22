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
        this->content = utils::stringify(content);
        this->date = utils::stringify(utils::random_date());
        this->sender = sender;
        this->receiver = receiver;
    }
    void to_sql() const{
        FILE* f = fopen(MESSAGE_OUTPUT, "a");
        fprintf(f, USER_MESSAGE_TEMPLATE, this->messageID, this->content.c_str(), this->date.c_str());
        fprintf(f, USER_CONVERSATION_TEMPLATE, this->messageID, sender->get_id(), receiver->get_id());
        fclose(f);
    }
};

#endif //SQL_POPULATOR_MESSAGE_H
