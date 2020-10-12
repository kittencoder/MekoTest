#ifndef EXTERNALS_H
#define EXTERNALS_H
#include <math.h>

#include <chrono>
#include <functional>
#include <future>
#include <iostream>
#include <queue>
#include <set>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <regex>

#include <filesystem>

#include <limits.h>
#include <stdlib.h>

#include <sys/inotify.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#include <type_traits>

// some helpful things:

// extracting keys/values from maps
template <typename MapType>
std::vector<typename MapType::key_type> extractKeys(const MapType& m) {
    std::vector<typename MapType::key_type> r;
    r.reserve(m.size());
    for (const auto& i : m) {
        r.push_back(i.first);
    }
    return r;
}

template <typename MapType>
std::vector<typename MapType::mapped_type> extractValues(const MapType& m) {
    std::vector<typename MapType::mapped_type> r;
    r.reserve(m.size());
    for (const auto& i : m) {
        r.push_back(i.second);
    }
    return r;
}

#endif