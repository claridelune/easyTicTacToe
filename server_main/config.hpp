#ifndef SERVER_MAIN_CONFIG_H
#define SERVER_MAIN_CONFIG_H

#include <string>
#include <vector>
#include <algorithm>

struct ConfigProps {
    std::string identity;
    std::string ipAddress;
    int isLeader = false;
};

class Config {
    private:
        std::vector<ConfigProps> _records;

    public:
        Config();
        ~Config();

        void set(const ConfigProps options);
        std::vector<ConfigProps>::iterator get(const std::string identity);
        void setLeader(const std::string identity);
        void remove(const std::string identity);
        std::vector<ConfigProps> all();
};

#endif
