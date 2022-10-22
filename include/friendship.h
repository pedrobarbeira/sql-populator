#ifndef SQL_POPULATOR_FRIENDSHIP_H
#define SQL_POPULATOR_FRIENDSHIP_H

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>
#include <cstdio>
#include "message.h"

class Friendship{
    Publisher* user1;
    Publisher* user2;
    std::vector<Message*> messages;
public:
    Friendship(Publisher* user1, Publisher* user2){
        this->user1 = user1;
        this->user2 = user2;
    }

    void to_sql(bool terminal) {
        FILE *file = fopen(RELATIONSHIP_OUTPUT, "a");
        fprintf(file, RELATIONSHIP_TEMPLATE, this->user1->get_id(), this->user2->get_id(), utils::stringify("Friend").c_str());
        for(auto i : messages){
            i->to_sql();
        }
        fclose(file);
    };

    [[nodiscard]] Publisher* get_user_1() const{
        return user1;
    }

    [[nodiscard]] Publisher* get_user_2() const{
        return user2;
    }

    void set_messages(std::vector<Message*> messages){
        this->messages = messages;
    }
};

#endif //SQL_POPULATOR_FRIENDSHIP_H
