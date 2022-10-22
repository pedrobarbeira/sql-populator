#ifndef SQL_POPULATOR_CONFIGS_H
#define SQL_POPULATOR_CONFIGS_H

#define NUM_USERS 100
#define MAX_FRIENDS NUM_USERS / 10
#define PRINT_TO_TERMINAL true
#define ARTICLE_LOWER_BOUND 800
#define ARTICLE_UPPER_BOUND 1200

#define USER_OUTPUT "user-data.txt"
#define RELATIONSHIP_OUTPUT "relationship-data.txt"

#define NAMES_FILE "data/babynames-clean.csv"
#define SURNAMES_FILE "data/Common_Surnames_Census_2000.csv"
#define SENTENCES_FILE "data/eng_sentences.tsv"

#define NAMES_KEY "names"
#define SURNAMES_KEY "surnames"
#define SENTENCES_KEY "sentences"

#define USER_TEMPLATE "INSERT INTO publisher(userID,name,password,creationDate,deletionDate,nFriends,posts,bio,reputation,banned)\nVALUES(%s,%s,%s,%s,%s,%s,%d,%d,%s,%d,%s)\n"
#define RELATIONSHIP_TEMPLATE "INSERT INTO relationship(relationshpID)\nVALUES(%s)\n"
#define FRIENDSHIP_TEMPLATE "INSERT INTO friendship(relationshipID, publiser1ID, publisher2ID)\nVALUES(%s,%s,%s)\n"

#endif //SQL_POPULATOR_CONFIGS_H
