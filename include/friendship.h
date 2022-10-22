#ifndef SQL_POPULATOR_FRIENDSHIP_H
#define SQL_POPULATOR_FRIENDSHIP_H

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>
#include <cstdio>
#include "message.h"

static uint32_t relationshipIdCount = 0;

class Friendship{
    uint32_t relationshipID;
    Publisher* user1;
    Publisher* user2;
    std::vector<Message*> messages;
public:
    Friendship(Publisher* user1, Publisher* user2){
        this->relationshipID = ++relationshipIdCount;
        this->user1 = user1;
        this->user2 = user2;
    }

    void to_sql(bool terminal){
        if(terminal){
            printf(FRIENDSHIP_TEMPLATE, this->relationshipID, this->user1->get_id(), this->user2->get_id());
        }
        else{
            FILE* file = fopen(RELATIONSHIP_OUTPUT, "a");
            fprintf(file, FRIENDSHIP_TEMPLATE, this->relationshipID, this->user1->get_id(), this->user2->get_id());
        }
    };

    Publisher* get_user_1() const{
        return user1;
    }

    Publisher* get_user_2() const{
        return user2;
    }

    void set_messages(std::vector<Message*> messages){
        this->messages = messages;
    }
};

#endif //SQL_POPULATOR_FRIENDSHIP_H
