#include "utils.hpp"

/* std::string getCurrentTime() {
    auto now = std::time(nullptr);
    std::ostringstream timeStream;
    timeStream << std::put_time(std::localtime(&now), "%Y-%m-%d %H:%M:%S");
    return timeStream.str();
} */

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
    char str[4];
};

int binaryToInt(const std::string &binaryNumber)
{
    assert(binaryNumber.size() == 4);

    merge x;
    for (int i = 0; i < 4; i++)
        x.str[i] = binaryNumber[i];

    return x.num;
}

std::string intToBinary(int num)
{
    std::string out;
    out.resize(4);

    merge x;
    x.num = num;

    for (int i = 0; i < 4; i++)
        out[i] = x.str[i];

    return out;
}