#include "config.hpp"

Config::Config() {}

Config::~Config() {}

void Config::set(const ConfigProps options) {
    _records.push_back(options);
}

std::vector<ConfigProps>::iterator Config::get(const std::string identity) {
    return std::find_if(_records.begin(), _records.end(), [&](const ConfigProps& item) {
        return item.identity == identity;
    });
}

void Config::setLeader(const std::string identity) {
    for(auto& trainer : _records) {
        trainer.isLeader = false;
    }

    auto record = get(identity);

    if (record != _records.end()) {
        record->isLeader = true;
    }
}

std::vector<ConfigProps> Config::all() {
    return _records;
}