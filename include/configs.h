#ifndef SQL_POPULATOR_CONFIGS_H
#define SQL_POPULATOR_CONFIGS_H

#define MAX_USERS 100
#define MAX_FRIENDS MAX_USERS / 10
#define MAX_COMMENT_CHAR 500
#define MAX_TOPICS 100
#define MAX_SEVERITY 7
#define MAX_REPORTS
#define MIN_ARTICLE_WORDS 700
#define PRINT_TO_TERMINAL true

#define USER_OUTPUT "user-data.txt"
#define RELATIONSHIP_OUTPUT "relationship-data.txt"

#define NAMES_FILE "data/names.csv"
#define SURNAMES_FILE "data/surnames.csv"
#define SENTENCES_FILE "data/sentences.tsv"
#define CATEGORIES_FILE "data/categories.txt"
#define TOPICS_FILE "data/topics.txt"
#define REASONS_FILE "data/reasons.txt"

#define NAMES_KEY "names"
#define SURNAMES_KEY "surnames"
#define SENTENCES_KEY "sentences"
#define CATEGORIES_KEY "categories"

#define USER_TEMPLATE "INSERT INTO publisher(userID,name,password,creationDate,deletionDate,nFriends,posts,bio,reputation,banned)\nVALUES(%s,%s,%s,%s,%s,%s,%d,%d,%s,%d,%s)\n"
#define FRIENDSHIP_TEMPLATE "INSERT INTO relationship(publiser1ID, publisher2ID)\nVALUES(%s,%s,%s)\n"
#define ARTICLE_TEMPLATE "INSERT INTO article(postID,nlikes,dislikes,ncomments,postdate,title,articleDescription,body,accepeted\nVALUES(%s,%d,%d,%d,%s,%s,%s,%s,%s)\n"
#define WRITE_ARTICLE_TEMPLATE "INSERT INTO write_article(userID, postID)\nVALUES(%s,%s)\n"
#define COMMENT_TEMPLATE "INSERT INTO comment(postID,userID,nlikes,ndislikes,ncomments,postdate,postID,content)\nVALUES(%s,%d,%d,%d,%d,%s,%s,%s)\n"
#define WRITE_COMMENT_TEMPLATE "INSERT INTO write_comment(userID, postID)\nVALUES(%s,%s)\n"

#endif //SQL_POPULATOR_CONFIGS_H
