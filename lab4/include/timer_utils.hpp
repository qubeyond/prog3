#pragma once

#include <chrono>
#include <string>

class TimeTracker {
   public:
    TimeTracker() noexcept;
    void start() noexcept;
    void stop() noexcept;
    double elapsed_milliseconds() const noexcept;
    double elapsed_microseconds() const noexcept;
    void print_elapsed(const std::string& description) const;

   private:
    std::chrono::high_resolution_clock::time_point begin_time;
    std::chrono::high_resolution_clock::time_point finish_time;
    bool is_active;
};