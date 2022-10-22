cmake_minimum_required(VERSION 3.23)
project(sql_populator)

set(CMAKE_CXX_STANDARD 23)

set(Boost_INCLUDE_DIR /usr/local/include/boost)
set(Boost_LIBRARY_DIR /usr/local/lib)
find_package(Boost 1.80.0)
include_directories(${Boost_INCLUDE_DIR})

if(NOT Boost_FOUND)
    message(FATAL_ERROR "Could not find boost!")
endif()

add_executable(sql_populator main.cpp include/publisher.h src/mockdata.cpp include/mockdata.h include/exceptions.h include/friendship.h include/configs.h include/article.h include/message.h include/utils.h src/utils.cpp include/category.h include/reports.h)
