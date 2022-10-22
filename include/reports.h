//
// Created by pbarbeira on 22-10-2022.
//

#ifndef SQL_POPULATOR_REPORTS_H
#define SQL_POPULATOR_REPORTS_H

#include "utils.h"
#include "configs.h"
#include "article.h"

class ReportReason;

static std::vector<ReportReason*> globalReasons;

static uint32_t reportReasonIdCount = 0;
static uint32_t userReportIdCount = 0;

class ReportReason{
    uint32_t reasonID;
    std::string reasonDescription;
    uint32_t severity;
public:
    explicit ReportReason(const std::string& reasonDescription){
        this->reasonID = ++reportReasonIdCount;
        this->reasonDescription = reasonDescription;
        this->severity = utils::random_uint32(MAX_SEVERITY);
    }

    static std::vector<ReportReason*> get_reasons(){
        if(globalReasons.empty()){
            std::cout << "[LOG] Loading report reasons\n";
            std::ifstream file(REASONS_FILE);
            if (file.fail()) {
                throw FileNotFoundException(REASONS_FILE);
            }
            std::string line;
            while (!file.eof()) {
                getline(file, line);
                auto reason = new ReportReason(line);
                globalReasons.push_back(reason);
            }
            file.close();
        }
        return globalReasons;
    }

    std::string to_sql() const;
};

class UserReport{
    uint32_t reportID;
    ReportReason* reason;
    Post* post;
    Publisher* reporter;
    Publisher* reportee;
public:
    explicit UserReport(ReportReason* reason, Post* post, Publisher* reporter){
        this->reportID = ++userReportIdCount;
        this->reason = reason;
        this->post = post;
        this->reporter = reporter;
        this->reportee = post->get_author();
    }
    std::string to_sql() const;
};

#endif //SQL_POPULATOR_REPORTS_H
