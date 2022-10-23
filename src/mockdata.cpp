#include "../include/mockdata.h"

template<typename T, typename K>
void mockdata::mockdata<T, K>::load_name_data() {
    file = std::ifstream(NAMES_FILE);
    if(file.fail()){
        throw FileNotFoundException(NAMES_FILE);
    }
    std::string line;
    while(!file.eof()){
        getline(file, line);
        try {
            std::string temp = utils::extract_csv(line);
            utils::name_normalizer(temp);
            if(data.contains(NAMES_KEY)){
                data.find(NAMES_KEY)->second.push_back(temp);
            }
            else{
                std::vector<std::string> v = {temp};
                data.insert(std::pair<std::string, std::vector<std::string>>(NAMES_KEY, v));
            }
        }
        catch(const NotCsvException& e){
            throw FileNotCsvException(NAMES_FILE);
        }
    }
    file.close();
}

template<typename T, typename K>
void mockdata::mockdata<T, K>::load_surname_data() {
    file = std::ifstream(SURNAMES_FILE);
    if(file.fail()){
        throw FileNotFoundException(SURNAMES_FILE);
    }
    std::string line;
    while(!file.eof()){
        getline(file, line);
        try {
            std::string temp = utils::extract_csv(line);
            utils::name_normalizer(temp);
            if(data.contains(SURNAMES_KEY)){
                data.find(SURNAMES_KEY)->second.push_back(temp);
            }
            else{
                std::vector<std::string> v = {temp};
                data.insert(std::pair<std::string, std::vector<std::string>>(SURNAMES_KEY, v));
            }
        }
        catch(const NotCsvException& e){
            throw FileNotCsvException(SURNAMES_FILE);
        }
    }
    file.close();
}

template<typename T, typename K>
void mockdata::mockdata<T, K>::load_publisher_data(const std::vector<Publisher*>& pubs) {
    for(const auto& pub : pubs) {
        std::set<uint32_t> entry;
        this->data.insert(std::pair<uint32_t, std::set<uint32_t>>(pub->get_id(), entry));
    }

}

template<typename T, typename K>
void mockdata::mockdata<T, K>::load_sentence_data() {
    file = std::ifstream(SENTENCES_FILE);
    if(file.fail()){
        throw FileNotFoundException(SENTENCES_FILE);
    }
    std::string line;
    while(!file.eof()){
        getline(file, line);
        if(!line.empty()) {
            std::string temp;
            for (unsigned int i = line.size()-1; i > 0; i--) {
                if (line[i] == '\t') {
                    temp = line.substr(i+1);
                    break;
                }
            }
            if (!data.contains(SENTENCES_KEY)) {
                std::vector<std::string> v = {temp};
                data.insert(std::pair<std::string, std::vector<std::string>>(SENTENCES_KEY, v));
            } else {
                data.find(SENTENCES_KEY)->second.push_back(temp);
            }
        }
    }
}

template<typename T, typename K>
Publisher* mockdata::mockdata<T, K>::generate_user() {
    if (!data.contains(NAMES_KEY)) {
        try {
            load_name_data();
        }
        catch(const FileException& e){
            std::cout << e.what();
        }
    }
    if (!data.contains(SURNAMES_KEY)) {
        try{
            load_surname_data();
        }
        catch(const FileException& e){
            std::cout << e.what();
        }
    }
    while(true) {
        int size = data.find(NAMES_KEY)->second.size();
        uint32_t i = utils::random_uint32(size) - 1;
        std::string username = data.find(NAMES_KEY)->second[i];
        size = data.find(SURNAMES_KEY)->second.size();
        i = utils::random_uint32(size) - 1;
        username += data.find(SURNAMES_KEY)->second[i];
        if (username.size() < MAX_USERNAME_SIZE && !nameMap.contains(username)){
            nameMap.insert(username);
            std::vector<Topic*> feedTopic = generate_user_topics();
            std::vector<Topic *> proposedTopics = {};
            if(utils::random_bool()) {
                proposedTopics = generate_proposed_topics();
            }
            return new Publisher(username, feedTopic, proposedTopics);
        }
    }
}

template<typename T, typename K>
void mockdata::mockdata<T, K>::clear_name_data(){
    this->data.erase(NAMES_KEY);
    this->data.erase(SURNAMES_KEY);
    nameMap.clear();
}

template<typename T, typename K>
Friendship *mockdata::mockdata<T, K>::generate_friendship(Publisher* pub, const std::vector<Publisher*> &pubs) {
    if(!this->data.empty()){
        load_publisher_data(pubs);
    }
    bool flag = true;
    uint32_t i;
    while(flag) {
        i = utils::random_uint32(pubs.size()) - 1;
        if (pub->get_id() != pubs[i]->get_id() && pubs[i]->get_friends() < MAX_FRIENDS) {
            if (!this->data.contains(pub->get_id())) {
                if (!this->data.contains(pubs[i]->get_id())) {
                    std::set<uint32_t> entry;
                    entry.insert(pubs[i]->get_id());
                    this->data.insert(std::pair<uint32_t, std::set<uint32_t>>(pub->get_id(), entry));
                    flag = false;
                } else {
                    auto it = this->data.find(pubs[i]->get_id());
                    std::set<uint32_t> &set = it->second;
                    if (!set.contains(pub->get_id())) {
                        set.insert(pub->get_id());
                        it->second = set;
                        flag = false;
                    }
                }
            } else {
                if (!this->data.contains(pubs[i]->get_id())) {
                    auto it = this->data.find(pub->get_id());
                    std::set<uint32_t> &set = it->second;
                    if (!set.contains(pubs[i]->get_id())) {
                        set.insert(pubs[i]->get_id());
                        it->second = set;
                        flag = false;
                    }
                } else {
                    auto it = this->data.find(pubs[i]->get_id());
                    std::set<uint32_t> &set = it->second;
                    if (!set.contains(pub->get_id())) {
                        set.insert(pub->get_id());
                        it->second = set;
                        flag = false;
                    }
                }
            }
        }
    }
    pub->add_friend();
    pubs[i]->add_friend();
    return new Friendship(pub, pubs[i]);
}

template<typename T, typename K>
Article *mockdata::mockdata<T, K>::generate_article(const std::vector<Publisher*>& pubs) {
    if (!data.contains(SENTENCES_KEY)) {
        try {
            load_sentence_data();
        }
        catch (const FileNotFoundException &e) {
            std::cout << e.what();
        }
    }
    std::vector<std::string> sentences = data.find(SENTENCES_KEY)->second;
    unsigned int i = utils::random_uint32(sentences.size()) - 1;
    std::string title = sentences[i];
    std::stringstream ss;
    uint32_t count = 0;
    for(;count<10;count++){
        i = utils::random_uint32(sentences.size()) - 1;
        ss << sentences[i];
        if(!ss.str().empty()) ss << " ";
    }
    std::string description = ss.str();
    ss.clear();
    count = 0;
    while (count < MIN_ARTICLE_WORDS) {
        i = utils::random_uint32(sentences.size()) - 1;
        ss << sentences[i];
        if (!ss.str().empty()) ss << " ";
        count += utils::wcount(sentences[i]);
    }
    std::string body = ss.str();
    i = utils::random_uint32(pubs.size()) - 1;
    Publisher* author = pubs[i];
    std::vector<Category*> articleCategories = generate_article_categories();
    std::vector<Topic*> articleTopics = generate_article_topics(articleCategories);
    std::vector<Comment*> comments = generate_article_comments(pubs);
    return new Article(title, description, body, author, articleCategories, articleTopics, comments);
}

template<typename T, typename K>
std::vector<Message*> mockdata::mockdata<T, K>::generate_message(Friendship *f) {
    uint32_t i = utils::random_uint32(MAX_MESSAGES) - 1;
    uint32_t j = utils::random_uint32(MAX_MESSAGES) - 1;
    if(!data.contains(SENTENCES_KEY)){
        try{
            load_sentence_data();
        }catch(const FileNotFoundException& e){
            std::cout << e.what();
        }
    }
    uint32_t k;
    std::vector<std::string> sentences = data.find(SENTENCES_KEY)->second;
    std::vector<Message*> toReturn;
    for (;i>0;i--){
        k = utils::random_uint32(sentences.size()) - 1;
        auto message = new Message(f->get_user_1(), f->get_user_2(), sentences[k]);
        toReturn.push_back(message);
    }
    for (;j>0;j--){
        k = utils::random_uint32(sentences.size()) - 1;
        auto message = new Message(f->get_user_2(), f->get_user_1(), sentences[k]);
        toReturn.push_back(message);
    }
    return toReturn;
}

template<typename T, typename K>
std::vector<Topic *> mockdata::mockdata<T, K>::generate_user_topics() const {
    std::vector<Topic*> topics = Topic::get_topics();
    std::vector<Topic*> toReturn;
    uint32_t j;
    for(int i = 0; i < MAX_TOPICS; i++){
        j = utils::random_uint32(topics.size()) - 1;
        toReturn.push_back(topics[j]);
    }
    return toReturn;
}

template<typename T, typename K>
std::vector<Topic *> mockdata::mockdata<T, K>::generate_proposed_topics() const {
    std::vector<Topic*> topics = Topic::get_topics();
    std::vector<Topic*> toReturn;
    uint32_t j;
    uint32_t i = 0;
    while(i < utils::random_uint32(MAX_PROPOSED_TOPICS) - 1){
        j = utils::random_uint32(topics.size()) - 1;
        if(!topics[j]->isAccepted()) {
            toReturn.push_back(topics[j]);
            i++;
        }
    }
    return toReturn;
}

template<typename T, typename K>
std::vector<Category *> mockdata::mockdata<T, K>::generate_article_categories() const {
    std::vector<Category*> toReturn;
    std::vector<Category*> categories = Category::get_categories();
    uint32_t i = utils::random_uint32(MAX_ARTICLE_CATEGORIES);
    uint32_t j;
    for(;i>0;i--){
        j = utils::random_uint32(categories.size()) - 1;
        toReturn.push_back(categories[j]);
    }
    return toReturn;
}

template<typename T, typename K>
std::vector<Topic *>
mockdata::mockdata<T, K>::generate_article_topics(const std::vector<Category*> &articleCategories) const {
    std::vector<Topic*> toReturn;
    std::vector<Topic*> topics = Topic::get_topics();
    uint32_t i = utils::random_uint32(MAX_ARTICLE_TOPICS) - 1;
    uint32_t j;
    Topic* t;
    uint32_t retries = 5;
    while(i>0){
        j = utils::random_uint32(topics.size()) - 1;
        t = topics[j];
        for(auto c : articleCategories){
            if(t->contains(c)){
                toReturn.push_back(t);
                i--;
                break;
            }
            else{
                if (!retries){
                    retries = 5;
                    i--;
                    if(i <= 0){
                        break;
                    }
                }
                else{
                    retries--;
                }
            }
        }
    }
    return toReturn;
}

template<typename T, typename K>
Comment * mockdata::mockdata<T, K>::generate_comment(const std::vector<Publisher*>& pubs, bool answer) const {
    uint32_t commentSize = utils::random_uint32(MAX_COMMENT_CHAR) - 1;
    uint32_t size = 0, i;
    std::stringstream content;
    std::vector<std::string> sentences = data.find(SENTENCES_KEY)->second;
    while(size < commentSize){
        i = utils::random_uint32(sentences.size()) - 1;
        content << sentences[i];
        size += sentences[i].size();
        if(size < commentSize){
            content << " ";
        }
    }
    std::vector<Comment*> answers;
    if(answer){
        answers = generate_comment_answers(pubs, false);
    }
    i = utils::random_uint32(pubs.size()) - 1;
    return new Comment(content.str(), pubs[i], answers);
}

template<typename T, typename K>
std::vector<Comment *>
mockdata::mockdata<T, K>::generate_article_comments(const std::vector<Publisher*> &pubs) {
    std::vector<Comment*> toReturn;
    if(utils::random_bool()) {
        uint32_t i = utils::random_uint32(MAX_ARTICLE_COMMENTS) - 1;
        uint32_t j;
        while (i > 0) {
            toReturn.push_back(generate_comment(pubs, true));
            i--;
        }
    }
    return toReturn;
}

template<typename T, typename K>
std::vector<Comment *> mockdata::mockdata<T, K>::generate_comment_answers(const std::vector<Publisher *> &pubs, bool answer) const {
    std::vector<Comment*> toReturn;
    if(utils::random_bool()) {
        uint32_t i = utils::random_uint32(MAX_COMMENT_ANSWERS) - 1;
        uint32_t j;
        while (i > 0) {
            toReturn.push_back(generate_comment(pubs, answer));
            i--;
        }
    }
    return toReturn;
}

std::vector<Publisher*> mockdata::generate_users(uint32_t n){
    std::map<std::string, int> nameMap;
    mockdata<std::string, std::vector<std::string>> mock;
    std::vector<Publisher*> toReturn;
    for(int i = 1; i <= n; i++){
        toReturn.push_back(mock.generate_user());
    }
    mock.clear_name_data();
    return toReturn;
}

std::vector<Friendship*> mockdata::generate_relationships(const std::vector<Publisher*> &pubs) {
    std::vector<Friendship*> toReturn;
    mockdata<std::uint32_t, std::set<uint32_t>> mock;
    for(auto pub : pubs) {
        uint32_t i = utils::random_uint32(MAX_FRIENDS) - 1;
        while (i > 0) {
            toReturn.push_back(mock.generate_friendship(pub, pubs));
            i--;
        }
    }
    return toReturn;
}

std::vector<Article*> mockdata::generate_articles(uint32_t n, const std::vector<Publisher *> &pubs) {
    mockdata<std::string, std::vector<std::string>> mock;
    std::vector<Article*> articles;
    for(int i = 1;i<=n;i++){
        std::cout << "\t[LOG] Generating article [" << i << "/" << n << "]\n";
        articles.push_back(mock.generate_article(pubs));
    }
    return articles;
}

std::vector<Message*> mockdata::generate_messages(const std::vector<Friendship*> &friends) {
    std::vector<Message*> toReturn;
    mockdata<std::string, std::vector<std::string>> mock;
    for(int i = 1; i <= friends.size();i++){
        std::cout << "[LOG] Generating conversation [" << i << "/" << friends.size() << "]\n";
        std::vector<Message*> messages = mock.generate_message(friends[i]);
        friends[i]->set_messages(messages);
    }
    return toReturn;
}

std::vector<Favorite*>mockdata::generate_favorites(const std::vector<Article*> &articles, const std::vector<Publisher *> &pubs) {
    std::vector<Favorite*> toReturn;
    uint32_t k;
    for(int i = 1; i <= pubs.size(); i++){
        std::cout << "[LOG] Generating favorites [" << i << "/" << pubs.size() << "]\n";
        for(uint32_t i = 0; i < utils::random_uint32(MAX_FAVORITE_ARTICLES) - 1; i++){
            k = utils::random_uint32(articles.size()) - 1;
            auto favorite = new Favorite(pubs[i]->get_id(), articles[k]->get_id());
            toReturn.push_back(favorite);
        }
    }
    return toReturn;
}

std::vector<UserReport *>
mockdata::generate_reports(const std::vector<Post*> &posts, const std::vector<Publisher*> &pubs) {
    std::vector<UserReport*> toReturn;
    std::vector<ReportReason*> reasons = ReportReason::get_reasons();
    uint32_t j, l, m;
    for(uint32_t i = 0; i < MAX_REPORTED_POSTS; i++){
        std::cout << "[LOG] Generating report [" << i << "/" << MAX_REPORTED_POSTS << "]\n";
        j = utils::random_uint32(posts.size()) - 1;
        for(uint32_t k = 0; k < MAX_REPORTS; k++){
            l = utils::random_uint32(pubs.size()) - 1;
            m = utils::random_uint32(NUMBER_REASONS) - 1;
            auto report = new UserReport(reasons[m], posts[j], pubs[l]);
            toReturn.push_back(report);
        }
    }
    return toReturn;
}

std::vector<Post*> mockdata::get_posts(const std::vector<Article*>& articles){
    std::vector<Post*> toReturn;
    for(auto i : articles){
        toReturn.push_back(i);
        for(auto j : i -> get_comments()){
            toReturn.push_back(j);
        }
    }
    return toReturn;
}

void mockdata::generate_mock_data(){
    std::vector<Category*> categories = Category::get_categories();
    std::vector<Topic*> topics = Topic::get_topics();
    std::cout << "[LOG] Generating users\n";
    std::vector<Publisher*> pubs = generate_users(MAX_USERS);
    std::cout << "[LOG] Generating friendships\n";
    std::vector<Friendship*> friends = generate_relationships(pubs);
    std::cout << "[LOG] Generating articles\n";
    std::vector<Article*> articles = generate_articles(MAX_ARTICLES, pubs);
    std::cout << "[LOG] Generating messages\n";
    std::vector<Message*> messages = generate_messages(friends);
    std::cout << "[LOG] Generating reports\n";
    std::vector<Post*> posts = get_posts(articles);
    std::vector<UserReport*> reports = generate_reports(posts, pubs);
    std::cout << "[LOG] Generating favorites\n";
    std::vector<Favorite*> favorites = generate_favorites(articles, pubs);
    std::cout << "[LOG] Saving categories\n";
    for(auto i :categories){
        i-> to_sql();
    }
    std::cout << "[LOG] Saving topics\n";
    for(auto i :topics){
        i->to_sql();
    }
    std::cout << "[LOG] Saving publishers\n";
    for(auto i : pubs){
        i->to_sql();
    }
    std::cout << "[LOG] Saving friends\n";
    for(auto i : friends){
        i->to_sql();
    }
    std::cout << "[LOG] Saving articles\n";
    for(auto i : articles){
        i->to_sql();
    }
    std::cout << "[LOG] Saving messages\n";
    for(auto i : messages){
        i->to_sql();
    }
    std::cout << "[LOG] Saving reports\n";
    for(auto i : reports){
        i->to_sql();
    }
    std::cout << "[LOG] Saving favores\n";
    for(auto i : favorites){
        i->to_sql();
    }
    std::cout << "[LOG] Creating populate.sql\n";
    std::vector<std::string> data = {CATEGORIES_OUTPUT,
                                     TOPICS_OUTPUT,
                                     USER_OUTPUT,
                                     RELATIONSHIP_OUTPUT,
                                     ARTICLE_OUTPUT,
                                     COMMENT_OUTPUT,
                                     MESSAGE_OUTPUT,
                                     REPORT_OUTPUT,
                                     FAVORITE_OUTPUT};
    std::ifstream file;
    std::string line;
    std::ofstream final(POPULATE_OUTPUT);
    if(!final.is_open()){
        std::cout << "[LOG] Failed to open file " << POPULATE_OUTPUT << "\n";
    }
    for(auto i : data){
        file = std::ifstream(i);
        if(!file.is_open()){
            std::cout << "[LOG] Failed to open file " << i << "\n";
        }
        while(!file.eof()){
            std::getline(file, line);
            final << line;
        }
        file.close();
    }
    final.close();
    std::cout << "[LOG] DONE\n";
}
