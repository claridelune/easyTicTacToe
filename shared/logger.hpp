#ifndef SHARED_LOGGER_H
#define SHARED_LOGGER_H

#include <iostream>
#include <string>

class Logger {
    private:
        std::string _context;

        void log(const std::string& message, const std::string& logLevel);

    public: 
        Logger(const std::string& context);
        ~Logger(); 

        void info(const std::string& message);
        void error(const std::string& message);
};

#endif