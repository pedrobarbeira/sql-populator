#include <iostream>
#include "include/mockdata.h"
#include "include/publisher.h"
#include "include/friendship.h"
#include "include/configs.h"

int main() {
//    std::vector<Publisher*> data;
//    std::vector<std::string> names = mockdata::generate_users(NUM_USERS);
//    for(const auto& name : names){;
//        data.emplace_back(new Publisher(name));
//    }
//    std::vector<Friendship*> friends = mockdata::generate_relationships(data);
//    for(auto p : data){
//        p->to_sql(PRINT_TO_TERMINAL);
//    }
//    for(auto f : friends){
//        f->to_sql(PRINT_TO_TERMINAL);
//    }

    mockdata::driver();

    return 0;
}
