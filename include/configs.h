#ifndef SQL_POPULATOR_CONFIGS_H
#define SQL_POPULATOR_CONFIGS_H

#define MAX_USERS 500
#define MAX_PROPOSED_TOPICS 2
#define MAX_FAVORITE_ARTICLES 20
#define MAX_ADMINS 10
#define MAX_FRIENDS MAX_USERS / 10
#define MAX_MESSAGES 10
#define MAX_ARTICLE_COMMENTS 10
#define MAX_COMMENT_ANSWERS 5
#define MAX_COMMENT_CHAR 500
#define MAX_TOPICS 100
#define MAX_SEVERITY 7
#define MAX_REPORTS
#define MAX_USERNAME_SIZE 20
#define MIN_ARTICLE_WORDS 700
#define MAX_ARTICLE_CATEGORIES 4
#define MAX_ARTICLE_TOPICS 10
#define MAX_REPORTS 50
#define MAX_REPORTED_POSTS 10
#define NUMBER_REASONS 5
#define PRINT_TO_TERMINAL true

#define USER_OUTPUT "user-data.txt"
#define RELATIONSHIP_OUTPUT "relationship-data.txt"
#define ARTICLE_OUTPUT "article-data.txt"
#define POPULATE_FILE "populate.sql"

#define NAMES_FILE "data/names.csv"
#define SURNAMES_FILE "data/surnames.csv"
#define SENTENCES_FILE "data/sentences.tsv"
#define CATEGORIES_FILE "data/categories.txt"
#define TOPICS_FILE "data/topics.txt"
#define REASONS_FILE "data/reasons.txt"

#define NAMES_KEY "names"
#define SURNAMES_KEY "surnames"
#define SENTENCES_KEY "sentences"

#define USER_TEMPLATE "INSERT INTO publisher(userID,name,password,creationDate,deletionDate,nFriends,posts,bio,reputation,banned)\nVALUES(%d,%s,%s,%s,%s,%s,%d,%d,%s,%d,%s)\n"
#define FRIENDSHIP_TEMPLATE "INSERT INTO relationship(relationshipID, publiser1ID, publisher2ID)\nVALUES(%d,%d,%d)\n"
#define ARTICLE_TEMPLATE "INSERT INTO article(postID,nlikes,dislikes,ncomments,postdate,title,articleDescription,body,accepeted\nVALUES(%s,%d,%d,%d,%s,%s,%s,%s,%s)\n"
#define WRITE_ARTICLE_TEMPLATE "INSERT INTO write_article(userID, postID)\nVALUES(%s,%s)\n"
#define COMMENT_TEMPLATE "INSERT INTO comment(postID,userID,nlikes,ndislikes,ncomments,postdate,postID,content)\nVALUES(%s,%d,%d,%d,%d,%s,%s,%s)\n"
#define WRITE_COMMENT_TEMPLATE "INSERT INTO write_comment(userID, postID)\nVALUES(%s,%s)\n"

#endif //SQL_POPULATOR_CONFIGS_H
