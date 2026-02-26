#pragma once

#include <string>
#include <chrono>

namespace Titan {

// Small utility helpers for logging and timing. Expandable.

class Logger {
public:
    enum Level { Info, Warn, Error };
    static void Log(Level lvl, const std::string& msg);
};

class ScopedTimer {
public:
    ScopedTimer(const std::string& name);
    ~ScopedTimer();

private:
    std::string name;
    std::chrono::steady_clock::time_point start;
};

} // namespace Titan
