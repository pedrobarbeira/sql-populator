#ifndef SQL_POPULATOR_FRIENDSHIP_H
#define SQL_POPULATOR_FRIENDSHIP_H

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>
#include <cstdio>

class Friendship{
    std::string relationshipID;
    std::string user1ID;
    std::string user2ID;
public:
    Friendship(const std::string& user1ID, const std::string& user2ID){
        this->relationshipID = utils::random_uuid();
        this->user1ID = user1ID;
        this->user2ID = user2ID;
    }
    void to_sql(bool terminal){
        if(terminal){
            printf(FRIENDSHIP_TEMPLATE, this->relationshipID.c_str(), this->user1ID.c_str(), this->user2ID.c_str());
        }
        else{
            FILE* file = fopen(RELATIONSHIP_OUTPUT, "a");
            fprintf(file, FRIENDSHIP_TEMPLATE, this->relationshipID.c_str(), this->user1ID.c_str(), this->user2ID.c_str());
        }

    };
};

#endif //SQL_POPULATOR_FRIENDSHIP_H
