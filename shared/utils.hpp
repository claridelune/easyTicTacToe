#ifndef SHARED_UTILS_H
#define SHARED_UTILS_H

#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <random>
#include <functional>
#include <cstdint>
#include <cassert>

#define PROTOCOL_SIZE 4

std::string uuid();

uint8_t uuidTohash(const std::string &uuid);

int binaryToInt(const std::string& binaryNumber);

std::string intToBinary(int num);

int hexToInt(const std::string& value);

std::string intToHex(int value);

#endif
