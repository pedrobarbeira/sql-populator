#include "../include/mockdata.h"

template<typename T, typename K>
void mockdata::mockdata<T, K>::load_name_data() {
    std::cout << "[LOG] Loading name data\n";
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
    std::cout << "[LOG] Loading surname data\n";
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
    std::cout << "[LOG] Loading sentence data\n";
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
        uint32_t i = utils::random_uint32(size);
        std::string username = data.find(NAMES_KEY)->second[i];
        size = data.find(SURNAMES_KEY)->second.size();
        i = utils::random_uint32(size);
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
        //TODO review this
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
                        flag = false;
                    }
                }
            } else {
                if (!this->data.contains(pubs[i]->get_id())) {
                    auto it = this->data.find(pub->get_id());
                    std::set<uint32_t> &set = it->second;
                    if (!set.contains(pubs[i]->get_id())) {
                        set.insert(pubs[i]->get_id());
                        flag = false;
                    }
                } else {
                    auto it = this->data.find(pubs[i]->get_id());
                    std::set<uint32_t> &set = it->second;
                    if (!set.contains(pub->get_id())) {
                        set.insert(pub->get_id());
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
    unsigned int i = utils::random_uint32(sentences.size());
    std::string title = sentences[i];
    std::stringstream ss;
    uint32_t count = 0;
    for(;count<10;count++){
        i = utils::random_uint32(sentences.size());
        ss << sentences[i];
        if(!ss.str().empty()) ss << " ";
    }
    std::string description = ss.str();
    ss.clear();
    count = 0;
    while (count < MIN_ARTICLE_WORDS) {
        i = utils::random_uint32(sentences.size());
        ss << sentences[i];
        if (!ss.str().empty()) ss << " ";
        count += utils::wcount(sentences[i]);
    }
    std::string body = ss.str();
    i = utils::random_uint32(pubs.size());
    Publisher* author = pubs[i];
    std::vector<Category*> articleCategories = generate_article_categories();
    std::vector<Topic*> articleTopics = generate_article_topics(articleCategories);
    std::vector<Comment*> comments;// = generate_article_comments(pubs);
    return new Article(title, description, body, author, articleCategories, articleTopics, comments);
}

template<typename T, typename K>
std::vector<Message*> mockdata::mockdata<T, K>::generate_message(Friendship *f) {
    uint32_t i = utils::random_uint32(MAX_MESSAGES);
    uint32_t j = utils::random_uint32(MAX_MESSAGES);
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
        k = utils::random_uint32(sentences.size());
        auto message = new Message(f->get_user_1(), f->get_user_2(), sentences[k]);
        toReturn.push_back(message);
    }
    for (;j>0;j--){
        k = utils::random_uint32(sentences.size());
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
        j = utils::random_uint32(topics.size());
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
    while(i < utils::random_uint32(MAX_PROPOSED_TOPICS)){
        j = utils::random_uint32(topics.size());
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
        j = utils::random_uint32(categories.size());
        toReturn.push_back(categories[j]);
    }
    return toReturn;
}

template<typename T, typename K>
std::vector<Topic *>
mockdata::mockdata<T, K>::generate_article_topics(const std::vector<Category *> &articleCategories) const {
    std::vector<Topic*> toReturn;
    std::vector<Topic*> topics = Topic::get_topics();
    uint32_t i = utils::random_uint32(MAX_ARTICLE_TOPICS);
    uint32_t j;
    Topic* t;
    while(i>0){
        j = utils::random_uint32(topics.size());
        t = topics[j];
        for(auto c : articleCategories){
            if(t->contains(c)){
                toReturn.push_back(t);
                i--;
                break;
            }
        }
    }
    return toReturn;
}

std::vector<Publisher*> mockdata::generate_users(uint32_t n){
    std::map<std::string, int> nameMap;
    mockdata<std::string, std::vector<std::string>> mock;
    std::vector<Publisher*> toReturn;
    for(int i = 1; i <= n; i++){
        std::cout << "[LOG] Generating user " << i << "\n";
        toReturn.push_back(mock.generate_user());
    }
    std::cout << "[LOG] Clearing name data\n";
    mock.clear_name_data();
    return toReturn;
}

std::vector<Friendship*> mockdata::generate_relationships(const std::vector<Publisher*> &pubs) {
    std::vector<Friendship*> toReturn;
    mockdata<std::uint32_t, std::set<uint32_t>> mock;
    for(auto pub : pubs) {
        uint32_t i = utils::random_uint32(MAX_FRIENDS);
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
        std::cout << "[LOG] Generating article " << i << "\n";
        articles.push_back(mock.generate_article(pubs));
    }
    return articles;
}

std::vector<Message*> mockdata::generate_messages(const std::vector<Friendship*> &friends) {
    std::vector<Message*> toReturn;
    mockdata<std::string, std::vector<std::string>> mock;
    for(auto i : friends){
        std::vector<Message*> messages = mock.generate_message(i);
        i->set_messages(messages);
    }
    return toReturn;
}

std::vector<Favorite*>mockdata::generate_favorites(const std::vector<Article *> &articles, const std::vector<Publisher *> &pubs) {
    std::vector<Favorite*> toReturn;
    uint32_t k;
    for(auto pub : pubs){
        for(uint32_t i = 0; i < utils::random_uint32(MAX_FAVORITE_ARTICLES); i++){
            k = utils::random_uint32(articles.size());
            auto favorite = new Favorite(pub->get_id(), articles[k]->get_id());
            toReturn.push_back(favorite);
        }
    }
    return toReturn;
}

std::vector<UserReport *>
mockdata::generate_reports(const std::vector<Post*> &posts, const bool& article, const std::vector<Publisher*> &pubs) {
    std::vector<UserReport*> toReturn;
    std::vector<ReportReason*> reasons = ReportReason::get_reasons();
    uint32_t j, l, m;
    for(uint32_t i = 0; i < MAX_REPORTED_POSTS; i++){
        j = utils::random_uint32(posts.size());
        for(uint32_t k = 0; k < MAX_REPORTS; k++){
            l = utils::random_uint32(pubs.size());
            m = utils::random_uint32(NUMBER_REASONS);
            auto report = new UserReport(reasons[m], posts[j], article, pubs[l]);
            toReturn.push_back(report);
        }
    }
    return toReturn;
}

void mockdata::generate_mock_data(){
    //Generates all data and writes to txt files;
}

void mockdata::sqlizer() {
    generate_mock_data();
    //appends txt files into populate.sql
}

