//
// Created by pbarbeira on 22-10-2022.
//

#ifndef SQL_POPULATOR_UTILS_H
#define SQL_POPULATOR_UTILS_H

#include <iostream>
#include <cstdint>
#include <random>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>
#include <openssl/sha.h>
#include <openssl/evp.h>
#include "exceptions.h"

namespace utils{
    uint32_t random_uint32(uint32_t max);
    std::string random_uuid();
    std::string random_date();
    bool random_bool();

    std::string stringify(const std::string& str);
    void all_lower(std::string& str);
    uint32_t wcount(const std::string& str);
    std::string extract_csv(std::string line);
    void name_normalizer(std::string& str);


}

#endif //SQL_POPULATOR_UTILS_H
