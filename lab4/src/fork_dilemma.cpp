#include "fork_dilemma.hpp"

#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <thread>

TableSimulation::TableSimulation(int n) : thinker_count(n), simulation_active(false) {
    for (int i = 0; i < n; ++i) {
        utensils.push_back(std::make_unique<std::mutex>());
    }
    meal_counter.resize(n, 0);
}

void TableSimulation::log_activity(int thinker_id, const std::string& activity) {
    std::lock_guard lock(console_lock);
    std::cout << "Мыслитель " << thinker_id << " " << activity << std::endl;
}

void TableSimulation::contemplate(int thinker_id) {
    log_activity(thinker_id, "размышляет...");
    std::this_thread::sleep_for(std::chrono::milliseconds(100 + rand() % 200));
}

void TableSimulation::consume_meal(int thinker_id) {
    log_activity(thinker_id, "принимает пищу.");
    meal_counter[thinker_id]++;
    std::this_thread::sleep_for(std::chrono::milliseconds(100 + rand() % 200));
}

void TableSimulation::thinker_routine(int id) {
    while (simulation_active.load()) {
        contemplate(id);
        if (!simulation_active.load()) break;

        int left = id;
        int right = (id + 1) % thinker_count;
        int first = std::min(left, right);
        int second = std::max(left, right);

        log_activity(
            id, "пытается взять приборы " + std::to_string(first) + " и " + std::to_string(second));

        {
            std::lock_guard lock1(*utensils[first]);
            log_activity(id, "взял прибор " + std::to_string(first));

            if (!simulation_active.load()) break;

            {
                std::lock_guard lock2(*utensils[second]);
                log_activity(id, "взял прибор " + std::to_string(second));
                consume_meal(id);
            }
            log_activity(id, "вернул прибор " + std::to_string(second));
        }
        log_activity(id, "вернул прибор " + std::to_string(first));
    }
}

void TableSimulation::start(int duration_sec) {
    std::cout << "ЗАДАЧА 3: Обедающие мыслители\n";

    simulation_active.store(true);
    std::vector<std::thread> thinkers;
    for (int i = 0; i < thinker_count; ++i) {
        thinkers.emplace_back(&TableSimulation::thinker_routine, this, i);
    }

    std::this_thread::sleep_for(std::chrono::seconds(duration_sec));
    simulation_active.store(false);

    for (auto& t : thinkers) t.join();

    std::cout << "\nСтатистика:\n";
    for (int i = 0; i < thinker_count; ++i) {
        std::cout << "Мыслитель " << i << ": " << meal_counter[i] << "\n";
    }
}

void execute_fork_dilemma() {
    int count, duration;
    std::cout << "Мыслителей: ";
    std::cin >> count;
    std::cout << "Секунд: ";
    std::cin >> duration;

    TableSimulation sim(count > 1 ? count : 5);
    sim.start(duration > 0 ? duration : 10);
}