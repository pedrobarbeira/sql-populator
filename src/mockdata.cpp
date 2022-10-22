#include "../include/mockdata.h"

void name_normalizer(std::string& str){
    for(int i = 0; i <str.size(); i++){
        if(i == 0) str[i] = toupper(str[i]);
        else str[i] = tolower(str[i]);
    }
}

std::string extract_csv(std::string line){
    for(auto i = 0; i < line.size(); i++){
        if (line[i] == ',') return line.substr(0, i);
    }
    throw NotCsvException();
}

template<typename T>
void mockdata::mockdata<T>::load_name_data() {
    file = std::ifstream(NAMES_FILE);
    if(file.fail()){
        throw FileNotFoundException(NAMES_FILE);
    }
    std::string line;
    while(!file.eof()){
        getline(file, line);
        try {
            std::string temp = extract_csv(line);
            name_normalizer(temp);
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
    file = std::ifstream(SURNAMES_FILE);
    if(file.fail()){
        throw FileNotFoundException(SURNAMES_FILE);
    }
    std::string line;
    while(!file.eof()){
        getline(file, line);
        try {
            std::string temp = extract_csv(line);
            name_normalizer(temp);
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
//"1276\teng\tLet's try something."

template<typename T>
std::pair<std::string, std::string> mockdata::mockdata<T>::generate_user() {
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
    std::pair<std::string, std::string> toReturn;
    std::random_device rd;
    int size =  data.find(NAMES_KEY)->second.size();
    std::uniform_int_distribution<int> names(1, size);
    toReturn.first = data.find(NAMES_KEY)->second[names(rd)];
    size =  data.find(SURNAMES_KEY)->second.size();
    std::uniform_int_distribution<int> surnames(1, size);
    toReturn.second = data.find(SURNAMES_KEY)->second[surnames(rd)];
    return toReturn;
}

template<typename T>
void mockdata::mockdata<T>::clear_name_data(){
    std::cout << "[LOG] Clearing names\n";
    this->data.erase(NAMES_KEY);
    if(!this->data.contains(NAMES_KEY))
        std::cout << "[LOG] Names Successfully cleared\n";
    std::cout << "[LOG] Clearing surnames\n";
    this->data.erase(SURNAMES_KEY);
    if(!this->data.contains(SURNAMES_KEY))
        std::cout << "[LOG] Surnames Successfully cleared\n";
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
        i = random_number(pubs.size()) - 1;
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


std::vector<std::string> mockdata::generate_users(uint32_t n){
    std::map<std::string, int> nameMap;
    mockdata<std::vector<std::string>> mock;
    int i = 0;
    while(i < n){
        std::pair<std::string, std::string> name = mock.generate_user();
        if(!nameMap.contains(name.first + name.second)) {
            nameMap.insert(std::pair<std::string, int>(name.first+name.second, 1));
            i++;
        }
    }
    std::cout << "[LOG] Clearing name data\n";
    mock.clear_name_data();
    std::vector<std::string> toReturn;
    for(const auto& name : nameMap){
        toReturn.push_back(name.first);
    }
    std::cout << "[LOG] Generated " << n << " unique names\n";
    return toReturn;
}

std::vector<Friendship*> mockdata::generate_relationships(const std::vector<Publisher*> &pubs) {
    std::vector<Friendship*> toReturn;
    mockdata<std::set<std::string>> mock;
    for(auto pub : pubs) {
        uint32_t i = random_number(MAX_FRIENDS);
        while (i > 0) {
            toReturn.push_back(mock.generate_friendship(*pub, pubs));
            i--;
        }
    }
    return toReturn;
}

template<typename T>
void mockdata::mockdata<T>::driver(){
    try{
        generate_article();
    }
    catch(const FileNotFoundException& e){
        std::cout << e.what();
    }
}

uint32_t wcount(const std::string& str){
    uint32_t count = 1;
    for(int i=0;i<str.size();i++){
        if(str[i] == ' ') count++;
    }
    return count;
}

template<typename T>
Article *mockdata::mockdata<T>::generate_article() {
    if (!data.contains(SENTENCES_KEY)) {
        try {
            load_sentence_data();
        }
        catch (const FileNotFoundException &e) {
            std::cout << e.what();
        }
    }
    std::cout << "[LOG] Generating article\n";
    std::stringstream ss;
    uint32_t count = 0;
    std::vector<std::string> lines = data.find(SENTENCES_KEY)->second;
    while (count< ARTICLE_LOWER_BOUND) {
        unsigned int i = random_number(lines.size());
        ss << lines[i];
        if (!ss.str().empty()) ss << " ";
        count += wcount(lines[i]);
        std::cout << count << std::endl;
    }
    std::cout << ss.str();
    return nullptr;
}

void mockdata::driver(){
    mockdata<std::vector<std::string>> mock;
    mock.driver();
}