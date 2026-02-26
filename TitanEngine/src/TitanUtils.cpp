#include "../include/TitanUtils.hpp"
#include <iostream>

namespace Titan {

void Logger::Log(Level lvl, const std::string& msg) {
    const char* prefix = "[INFO]";
    if (lvl == Warn) prefix = "[WARN]";
    if (lvl == Error) prefix = "[ERROR]";
    std::cout << prefix << " " << msg << std::endl;
}

ScopedTimer::ScopedTimer(const std::string& name_) : name(name_), start(std::chrono::steady_clock::now()) {}

ScopedTimer::~ScopedTimer() {
    auto end = std::chrono::steady_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "[TIMER] " << name << " took " << ms << " ms" << std::endl;
}

} // namespace Titan
