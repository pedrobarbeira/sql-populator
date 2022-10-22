#ifndef SQL_POPULATOR_EXCEPTIONS_H
#define SQL_POPULATOR_EXCEPTIONS_H

#include <exception>
#include <string>
#include <sstream>

class FileException : public std::exception{
protected:
    std::string err;
public:
    [[nodiscard]] const char* what() const noexcept override{
        return err.c_str();
    }
};

class FileNotFoundException : public FileException {
public:
    explicit FileNotFoundException(const std::string& filename){
        this->err = "Failed to open file [" + filename + "]\n";
    }
};

class FileNotCsvException : public FileException{
public:
    FileNotCsvException(const std::string& filename){
        this->err = "File [" + filename + "] is not in csv format\n";
    }
};

class NotCsvException : public std::exception {
    std::string err;
public:
    NotCsvException(){
        this->err = "Line isn't in CSV\n";
    }
    [[nodiscard]] const char* what() const noexcept override{
        return err.c_str();
    }
};

#endif //SQL_POPULATOR_EXCEPTIONS_H
