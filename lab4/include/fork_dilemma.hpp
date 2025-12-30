#pragma once

#include <atomic>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

class TableSimulation {
   public:
    explicit TableSimulation(int n);
    void start(int duration_sec);

   private:
    void log_activity(int thinker_id, const std::string& activity);
    void contemplate(int thinker_id);
    void consume_meal(int thinker_id);
    void thinker_routine(int id);

    int thinker_count;
    std::vector<std::unique_ptr<std::mutex>> utensils;
    std::vector<int> meal_counter;
    std::mutex console_lock;
    std::atomic<bool> simulation_active;
};

void execute_fork_dilemma();