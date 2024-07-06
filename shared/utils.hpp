#ifndef SHARED_UTILS_H
#define SHARED_UTILS_H

#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <random>
#include <functional>
#include <cstdint>

// std::string getCurrentTime();

std::string uuid();

uint8_t uuidTohash(const std::string &uuid);
#endif
