#include "timer_utils.hpp"

#include <iostream>

TimeTracker::TimeTracker() noexcept : is_active(false) {
}

void TimeTracker::start() noexcept {
    begin_time = std::chrono::high_resolution_clock::now();
    is_active = true;
}

void TimeTracker::stop() noexcept {
    finish_time = std::chrono::high_resolution_clock::now();
    is_active = false;
}

double TimeTracker::elapsed_milliseconds() const noexcept {
    auto finish = is_active ? std::chrono::high_resolution_clock::now() : finish_time;
    return std::chrono::duration<double, std::milli>(finish - begin_time).count();
}

double TimeTracker::elapsed_microseconds() const noexcept {
    auto finish = is_active ? std::chrono::high_resolution_clock::now() : finish_time;
    return std::chrono::duration<double, std::micro>(finish - begin_time).count();
}

void TimeTracker::print_elapsed(const std::string& description) const {
    std::cout << description << ": " << elapsed_milliseconds() << " ms" << std::endl;
}