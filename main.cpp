#include <iostream>
#include "include/mockdata.h"
#include "include/publisher.h"
#include "include/friendship.h"
#include "include/configs.h"

int main() {
    std::vector<Publisher*> pubs= mockdata::generate_users(MAX_USERS);
    std::vector<Article*> articles;
    mockdata::generate_articles(100, pubs);
//    std::vector<Friendship*> friends = mockdata::generate_relationships(pubs);
//    for(auto p : pubs){
//        p->to_sql(PRINT_TO_TERMINAL);
//    }
//    for(auto f : friends){
//        f->to_sql(PRINT_TO_TERMINAL);
//    }

    return 0;
}
