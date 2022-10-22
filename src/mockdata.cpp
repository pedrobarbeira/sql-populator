#include "../include/mockdata.h"


template<typename T>
void mockdata::mockdata<T>::load_categories() {
    std::cout << "[LOG] Loading categories\n";
    file = std::ifstream(CATEGORIES_FILE);
    if(file.fail()){
        throw FileNotFoundException(CATEGORIES_FILE);
    }
    std::string line;
    while(!file.eof()){
        getline(file, line);
        auto* category = new Category(line);
        categories.push_back(category);
    }
    file.close();
}

template<typename T>
void mockdata::mockdata<T>::load_topics() {
    try {
        load_categories();
    } catch (const FileNotFoundException &e) {
        std::cout << e.what();
    }
    file = std::ifstream(TOPICS_FILE);
    if (file.fail()) {
        throw FileNotFoundException(TOPICS_FILE);
    }
    std::string line;
    std::vector<std::string> words;
    while (!file.eof()) {
        getline(file, line);
        words.push_back(line);
    }
    uint32_t j, k;
    for(int i = 0; i < MAX_TOPICS; i++){
        k = utils::random_uint32(words.size());
        std::set<Category*> topicCategories;
        for(j = 0; j < utils::random_uint32(categories.size()); j++){
            uint32_t x = utils::random_uint32(categories.size());
            topicCategories.insert(categories[x]);
        }
        auto topic = new Topic(words[k], topicCategories);
        topics.push_back(topic);
    }
}

template<typename T>
void mockdata::mockdata<T>::load_name_data() {
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

template<typename T>
void mockdata::mockdata<T>::load_surname_data() {
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

template<typename T>
void mockdata::mockdata<T>::load_publisher_data(const std::vector<Publisher*>& pubs) {
    for(const auto& pub : pubs) {
        std::set<std::string> entry;
        this->data.insert(std::pair<std::string, std::set<std::string>>(pub->get_id(), entry));
    }

}

template<typename T>
void mockdata::mockdata<T>::load_sentence_data() {
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

template<typename T>
Publisher* mockdata::mockdata<T>::generate_user() {
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
    if(topics.empty()) {
        try {
            load_topics();
        }catch (const FileNotFoundException& e){
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
        if (!nameMap.contains(username)){
            nameMap.insert(username);
            std::vector<Topic*> feedTopic = generate_user_topics();
            std::vector<Topic*> proposedTopics = generate_proposed_topics();
            return new Publisher(username, feedTopic, proposedTopics);
        }
    }
}

template<typename T>
void mockdata::mockdata<T>::clear_name_data(){
    this->data.erase(NAMES_KEY);
    this->data.erase(SURNAMES_KEY);
    nameMap.clear();
}

template<typename T>
Friendship *mockdata::mockdata<T>::generate_friendship(Publisher &pub, const std::vector<Publisher*> &pubs) {
    if(!this->data.empty()){
        load_publisher_data(pubs);
    }
    bool flag = true;
    uint32_t i;
    while(flag) {
        //TODO review this
        i = utils::random_uint32(pubs.size()) - 1;
        if (pub.get_id() != pubs[i]->get_id() && pubs[i]->get_friends() < MAX_FRIENDS) {
            if (!this->data.contains(pub.get_id())) {
                if (!this->data.contains(pubs[i]->get_id())) {
                    std::set<std::string> entry;
                    entry.insert(pubs[i]->get_id());
                    this->data.insert(std::pair<std::string, std::set<std::string>>(pub.get_id(), entry));
                    flag = false;
                } else {
                    auto it = this->data.find(pubs[i]->get_id());
                    std::set<std::string> &set = it->second;
                    if (!set.contains(pub.get_id())) {
                        set.insert(pub.get_id());
                        flag = false;
                    }
                }
            } else {
                if (!this->data.contains(pubs[i]->get_id())) {
                    auto it = this->data.find(pub.get_id());
                    std::set<std::string> &set = it->second;
                    if (!set.contains(pubs[i]->get_id())) {
                        set.insert(pubs[i]->get_id());
                        flag = false;
                    }
                } else {
                    auto it = this->data.find(pubs[i]->get_id());
                    std::set<std::string> &set = it->second;
                    if (!set.contains(pub.get_id())) {
                        set.insert(pub.get_id());
                        flag = false;
                    }
                }
            }
        }
    }
    pub.add_friend();
    pubs[i]->add_friend();
    return new Friendship(pub.get_id(), pubs[i]->get_id());
}


std::vector<Publisher*> mockdata::generate_users(uint32_t n){
    std::map<std::string, int> nameMap;
    mockdata<std::vector<std::string>> mock;
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
    mockdata<std::set<std::string>> mock;
    for(auto pub : pubs) {
        uint32_t i = utils::random_uint32(MAX_FRIENDS);
        while (i > 0) {
            toReturn.push_back(mock.generate_friendship(*pub, pubs));
            i--;
        }
    }
    return toReturn;
}

template<typename T>
Article *mockdata::mockdata<T>::generate_article(const std::vector<Publisher*>& pubs) {
    if (!data.contains(SENTENCES_KEY)) {
        try {
            load_sentence_data();
        }
        catch (const FileNotFoundException &e) {
            std::cout << e.what();
        }
    }
    if(categories.empty()) {
        try {
            load_topics();
        }catch (const FileNotFoundException& e){
            std::cout << e.what();
        }
    }
    std::vector<std::string> lines = data.find(SENTENCES_KEY)->second;
    unsigned int i = utils::random_uint32(lines.size());
    std::string title = lines[i];
    std::stringstream ss;
    uint32_t count = 0;
    for(;count<10;count++){
        i = utils::random_uint32(lines.size());
        ss << lines[i];
        if(!ss.str().empty()) ss << " ";
    }
    std::string description = ss.str();
    ss.clear();
    count = 0;
    while (count < MIN_ARTICLE_WORDS) {
        i = utils::random_uint32(lines.size());
        ss << lines[i];
        if (!ss.str().empty()) ss << " ";
        count += utils::wcount(lines[i]);
    }
    std::string body = ss.str();
    i = utils::random_uint32(pubs.size());
    Publisher* author = pubs[i];
    std::vector<Category*> categories = generate_article_categories();
    std::vector<Topic*> topics = generate_article_topics(categories);
    return new Article(title, description, body, author, categories, topics);
}

std::vector<Article*> mockdata::generate_articles(uint32_t n, const std::vector<Publisher *> &pubs) {
    mockdata<std::vector<std::string>> mock;
    std::vector<Article*> articles;
    for(int i = 1;i<=n;i++){
        std::cout << "[LOG] Generating article " << i << "\n";
        articles.push_back(mock.generate_article(pubs));
    }
    return articles;
}
