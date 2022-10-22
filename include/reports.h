//
// Created by pbarbeira on 22-10-2022.
//

#ifndef SQL_POPULATOR_REPORTS_H
#define SQL_POPULATOR_REPORTS_H

#include "utils.h"
#include "configs.h"
#include "article.h"

class ReportReason{
    std::string reasonID;
    std::string reasonDescription;
    uint32_t severity;
public:
    ReportReason(const std::string& reasonDescription, uint32_t severity){
        this->reasonID = utils::random_uuid();
        this->reasonDescription = reasonDescription;
        this->severity = utils::random_uint32(MAX_SEVERITY);
    }
    std::string to_sql() const;
};

class UserReport{
    std::string reportID;
    ReportReason* reason;
    Post* post;
    Publisher* reporter;
    Publisher* reportee;
public:
    explicit UserReport(ReportReason* reason, Post* post, Publisher* reporter, Publisher* reportee){
        this->reportID = utils::random_uuid();
        this->reason = reason;
        this->post = post;
        this->reporter = reporter;
        this->reportee = reportee;
    }
    std::string to_sql() const;
};

#endif //SQL_POPULATOR_REPORTS_H
