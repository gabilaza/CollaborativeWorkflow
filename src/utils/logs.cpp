
#include <iostream>
#include <fstream>
#include <system_error>
#include <string>
#include "logs.hpp"

using namespace logger;

const char* logs::filepath = nullptr;


logs::logs(LogLevel level) {
    this -> level = level;
    if(filepath != nullptr) {
        (this -> fout).open(filepath, std::fstream::app);
        (this -> fout) << "[" << getLogLevelName(this -> level) << "] ";
    }
    std::cout << "[" << getLogLevelName(this -> level) << "] ";
}


logs::logs(const char* tag) : logs(DEBUG) {
    if(filepath != nullptr)
        (this -> fout) << "[" << tag << "] ";
    std::cout << "[" << tag << "] ";
}


logs::~logs() {
    std::cout << '\n';
    if(filepath != nullptr) {
        (this -> fout) << '\n';
        (this -> fout).close();
    }
}


const char* logs::getLogLevelName(LogLevel level) {
    switch(level) {
        case 0:
            return "DEBUG";
        case 1:
            return "INFO";
        case 2:
            return "WARNING";
        case 3:
            return "ERROR";
        case 4:
            return "CRITICIAL";
    }
    return "NONE";
}


void logs::logOnTheDisk(const char* newFilepath) {
    filepath = newFilepath;
}


std::string logger::makeMessage(const char *tag, const char *funcName) {
    std::string what;
    what += tag;
    what += "::";
    what += funcName;
    return what;
}


void logger::raiseSystemError(const char *tag, const char *funcName, int err) {
    std::string what = makeMessage(tag, funcName);
    logs(LogLevel::ERROR) << what;
    throw std::system_error(err, std::system_category(), what);
}


void logger::raiseError(const char *msg) {
    logs(LogLevel::ERROR) << msg;
    throw std::runtime_error(msg);
}

