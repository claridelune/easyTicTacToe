#include "utils.hpp"

std::string uuid() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 15);
    std::uniform_int_distribution<> dis2(8, 11);

    std::stringstream ss;
    ss << std::hex;
    for (int i = 0; i < 8; i++) {
        ss << dis(gen);
    }
    ss << "-";
    for (int i = 0; i < 4; i++) {
        ss << dis(gen);
    }
    ss << "-4";
    for (int i = 0; i < 3; i++) {
        ss << dis(gen);
    }
    ss << "-";
    ss << dis2(gen);
    for (int i = 0; i < 3; i++) {
        ss << dis(gen);
    }
    ss << "-";
    for (int i = 0; i < 12; i++) {
        ss << dis(gen);
    }

    return ss.str();
}

uint8_t uuidTohash(const std::string& uuid) {
    std::hash<std::string> hasher;
    size_t hashValue = hasher(uuid);

    return static_cast<uint8_t>(hashValue % 256);
}

union merge
{
    int num;
    char str[PROTOCOL_SIZE];
};

int binaryToInt(const std::string &binaryNumber)
{
    assert(binaryNumber.size() == PROTOCOL_SIZE);

    merge x;
    for (int i = 0; i < PROTOCOL_SIZE; i++)
        x.str[i] = binaryNumber[i];

    return x.num;
}

std::string intToBinary(int num)
{
    std::string out;
    out.resize(PROTOCOL_SIZE);

    merge x;
    x.num = num;

    for (int i = 0; i < PROTOCOL_SIZE; i++)
        out[i] = x.str[i];

    return out;
}

int hexToInt(const std::string& value) {
    int num;
    std::stringstream ss;
    ss << std::hex << value;
    ss >> num;
    return num;
}

std::string intToHex(int value) {
    std::stringstream ss;
    ss << std::hex << std::setw(PROTOCOL_SIZE) << std::setfill('0') << value;
    return ss.str();
}