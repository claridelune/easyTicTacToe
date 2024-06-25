#ifndef SHARED_DATABASE_H
#define SHARED_DATABASE_H

#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct UserCredential {
    std::string user;
    int role;
};

class Database {
    private:
        std::vector<UserCredential> _records;

    public:
        Database();
        ~Database();

        void initialize();
        std::vector<UserCredential> all();
        std::vector<UserCredential>::iterator find(std::string user);
};

#endif