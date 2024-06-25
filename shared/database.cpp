#include "database.hpp"

Database::Database() {
    initialize();
}

Database::~Database() {}

void Database::initialize() {
    if (!_records.empty())
        return;

    std::ifstream credentialsFile("credentials.json");
    if (!credentialsFile)
        throw std::runtime_error("Error when trying to read database.");

    json jsonMeta;
    credentialsFile >> jsonMeta;

    for(auto& entry : jsonMeta) {
        UserCredential credential;
        credential.user = entry["user"];
        credential.role = entry["role"];

        _records.push_back(credential);
    }
}

std::vector<UserCredential> Database::all() {
    return _records;
}

std::vector<UserCredential>::iterator Database::find(std::string user) {
    return std::find_if(_records.begin(), _records.end(), [&](const UserCredential& item) {
        return item.user == user;
    });
}