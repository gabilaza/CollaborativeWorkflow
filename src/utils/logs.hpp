#ifndef __LOGGER_H
#define __LOGGER_H

#include <iostream>
#include <fstream>
#include <string>

namespace logger {
    enum LogLevel {
        DEBUG, INFO, WARNING, ERROR, CRITICAL
    };

    class logs {
        private:
            LogLevel level;
            std::ofstream fout;
            static const char* filepath;
        public:
            logs(LogLevel level);
            logs(const char* tag);
            ~logs();
            static const char* getLogLevelName(LogLevel level);
            static void logOnTheDisk(const char* newFilepath);

            template <class T>
            logs& operator<<(const T& message) {
                std::cout << message;
                if(filepath != nullptr)
                    fout << message;
                return *this;
            }
    };

    std::string makeMessage(const char *tag, const char *funcName);
    void raiseSystemError(const char *tag, const char *funcName, int err = errno);
    void raiseError(const char *msg);
}

#endif
