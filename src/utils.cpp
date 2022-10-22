//
// Created by pbarbeira on 22-10-2022.
//

#include "../include/utils.h"

namespace utils {
    uint32_t random_uint32(uint32_t max) {
        std::random_device rd;
        std::uniform_int_distribution<int> dist(1, max);
        return dist(rd);
    }

    std::string stringify(const std::string& str) {
        return "\'" + str + "\'";
    }

    std::string random_date() {
        uint32_t year = 2020 + random_uint32(2);
        uint32_t month = 1 + random_uint32(11);
        uint32_t day;
        if (month == 2) day = 1 + random_uint32(27);
        else if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)
            day = 1 + random_uint32(30);
        else day = 1 + random_uint32(29);
        std::stringstream ss;
        ss << year << '-' << month << '-' << day;
        return ss.str();
    }

    void all_lower(std::string &str) {
        for (int i = 0; i < str.size(); i++) {
            if (isupper(str[i])) str[i] = tolower(str[i]);
        }
    }

    std::string random_uuid() {
        boost::uuids::uuid uuid = boost::uuids::random_generator()();
        return boost::lexical_cast<std::string>(uuid);
    }

    uint32_t wcount(const std::string& str){
        uint32_t count = 1;
        for(int i=0;i<str.size();i++){
            if(str[i] == ' ') count++;
        }
        return count;
    }

    bool random_bool(){
        uint32_t i = random_uint32(2);
        return i == 1;
    }

    std::string extract_csv(std::string line){
        for(auto i = 0; i < line.size(); i++){
            if (line[i] == ',') return line.substr(0, i);
        }
        throw NotCsvException();
    }

    void name_normalizer(std::string& str){
        for(int i = 0; i <str.size(); i++){
            if(i == 0) str[i] = toupper(str[i]);
            else str[i] = tolower(str[i]);
        }
    }
}