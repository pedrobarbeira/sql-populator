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
        this->reasonDescription = utils::stringify(reasonDescription);
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

    void to_sql() const{
        FILE* f = fopen(REASONS_FILE, "a");
        fprintf(f, REPORT_REASON_TEMPLATE, this->reasonID, this->reasonDescription.c_str(), this->severity);
        fclose(f);
    }

    uint32_t get_id() const{
        return this->reasonID;
    }
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

    void to_sql() const{
        FILE* f =fopen(REPORT_OUTPUT, "a");
        fprintf(f, USER_REPORT_TEMPLATE, this->reportID, this->reason->get_id());
        fprintf(f, REPORT_TEMPLATE, this->reportID, this->reporter->get_id(), this->reportee->get_id());
        if(this->post->get_type() == ARTICLE) {
            fprintf(f, REPORTED_ARTICLE_TEMPLATE, this->reportID, this->post->get_id());
        }
        else{
            fprintf(f, REPORTED_COMMENT_TEMPLATE, this->reportID, this->post->get_id());
        }
        fprintf(f, REASON_TEMPLATE, this->reportID, this->reason->get_id());
        fclose(f);
    }
};

#endif //SQL_POPULATOR_REPORTS_H
