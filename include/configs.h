#ifndef SQL_POPULATOR_CONFIGS_H
#define SQL_POPULATOR_CONFIGS_H

#define MAX_USERS 500
#define MAX_PROPOSED_TOPICS 2
#define MAX_FAVORITE_ARTICLES 20
#define MAX_ADMINS 10
#define MAX_FRIENDS MAX_USERS / 10
#define MAX_BANNED_USERS 5
#define MAX_MESSAGES 10
#define MAX_ARTICLE_COMMENTS 10
#define MAX_COMMENT_ANSWERS 5
#define MAX_COMMENT_CHAR 500
#define MAX_TOPICS 100
#define MAX_SEVERITY 7
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
#define MESSAGE_OUTPUT "message-data.txt"
#define REPORT_OUTPUT "reportdata.txt"
#define FAVORITE_OUTPUT "favorite-data.txt"
#define COMMENT_OUTPUT "comment-data.txt"
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

#define PUBLISHER_TEMPLATE "INSERT INTO publisher(userID, nome, email, publishPassword, creationDate, nfriends, profilePic, bio, publisherPosts, reputation, banned)\nVALUES(%d,%s,%s,%s,%s,%d,%s,%s,%d,%d,%s)\n"
#define ADMIN_TEMPLATE "INSERT INTO publisher(userID, nome, email, adminPassword, creationDate)\nVALUES(%d,%s,%s,%s,%s,%d)\n"
#define BAN_TEMPLATE "INSERT INTO ban(publisherID, adminID)\nVALUES(%d,%d)\n"
#define USER_MESSAGE_TEMPLATE "INSERT INTO user_message(messageID, content, messageDate)\nVALUES(%d,%s,%s)\n"
#define ARTICLE_TEMPLATE "INSERT INTO article(articleID, title, articleDescription, body, accepted, nLikes, nDislikes, nComments, postDate)\nVALUES(%d,%s,%s,%s,%s,%d,%d,%d,%s)\n"
#define COMMENT_TEMPLATE "INSERT INTO comment(commentID, content, nLikes, nDislikes, postDate)\nVALUES(%d,%s,%d,%d,%s)"
#define ANSWER_TEMPLATE "INSERT INTO answer(parentID, answerID)\nVALUES(%d,%d)"
#define CATEGORY_TEMPLATE "INSERT INTO category(categoryID, categoryName)\nVALUES(%d,%s)"
#define TOPIC_TEMPLATE "INSERT INTO topic(topicID, topicName, accepted)\nVALUES(%d,%s,%s)"
#define REPORT_REASON_TEMPLATE "INSERT INTO report_reason(reasonID, reasonDescription, severity)\nVALUES(%d,%s,%d)"
#define USER_REPORT_TEMPLATE "INSERT INTO user_report(reportID, reasonID)\nVALUES(%d,%d)"
#define REPORT_TEMPLATE "INSERT INTO report(reportID, reporterID, reportedID)\nVALUES(%d,%d,%d)\n"
#define NOTIFICATION_TEMPLATE "INSERT INTO notification(notificationID, viewed, jsonData)\nVALUES(%d,%d,%s)\n"
#define USER_NOTIFICATION_TEMPLATE "INSERT INTO user_notification(notificationID, userID)\nVALUES(%d,%d)"
#define RELATIONSHIP_TEMPLATE "INSERT INTO relationship(publisher1ID, publisher2ID)\nVALUES(%d,%d,%s)\n"
#define REASON_TEMPLATE "INSERT INTO reason(reportID, reasonID)\nVALUES(%d,%d)"
#define USER_CONVERSATION_TEMPLATE "INSERT INTO user_conversation(messageID, senderID, receiverID)\nVALUES(%d,%d,%d)\n"
#define FAVORITES_TEMPLATE "INSERT INTO favorites(publisherID, articleID)\nVALUES(%d,%d)"
#define FEED_TOPIC_TEMPLATE "INSERT INTO feed_topic(publisherID, topicID)\nVALUES(%d,%d)\n"
#define PROPOSE_TOPIC_TEMPLATE "INSERT INTO propose_topic(topicID, publisherID)\nVALUES(%d,%d)\n"
#define ARTICLE_HAS_TOPIC_TEMPLATE "INSERT INTO article_has_topic(articleID, topicID)\nVALUES(%d,%d)"
#define CATEGORY_TOPIC_TEMPLATE "INSERT INTO category_topics(topicID, categoryID)\nVALUES\n(%d,%d)"
#define WRITE_COMMENT_TEMPLATE "INSERT INTO write_comment(commentID, userID)\nVALUES(%d,%d)\n"
#define COMMENT_ARTICLE_TEMPLATE "INSERT INTO comment_article(commentID, articleID)\nVALUES(%d,%d)\n"
#define WRITE_ARTICLE_TEMPLATE "INSERT INTO write_article(userID, articleID)\nVALUES(%d,%d)\n"
#define ARTICLE_CATEGORY_TEMPLATE "INSERT INTO article_category(articleID, categoryID)\nVALUES(%d,%d)\n"
#define REPORTED_ARTICLE_TEMPLATE "INSERT INTO reported_article(reportID, articleID)\nVALUES(%d,%d)\n"
#define REPORTED_COMMENT_TEMPLATE "INSERT INTO report_comment(reportID, commentID)\nVALUES(%d,%d)\n"

#endif //SQL_POPULATOR_CONFIGS_H
