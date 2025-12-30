#include "concurrency_stress.hpp"
#include <functional>
#include <iostream>
#include <random>
#include <thread>
#include <vector>
#include "timer_utils.hpp"

char get_random_ascii_char() {
    static thread_local std::random_device rd;
    static thread_local std::mt19937 gen(rd());
    static thread_local std::uniform_int_distribution<> dis(33, 126);
    return static_cast<char>(dis(gen));
}

void worker_no_lock(int, std::string& buffer) {
    for (int i = 0; i < LOOPS_PER_WORKER; ++i) {
        buffer += get_random_ascii_char();
    }
}

void worker_with_mutex(int, std::string& buffer, std::mutex& lock) {
    for (int i = 0; i < LOOPS_PER_WORKER; ++i) {
        std::lock_guard guard(lock);
        buffer += get_random_ascii_char();
    }
}

void worker_with_atomic_lock(int, std::string& buffer, AtomicLock& spin) {
    for (int i = 0; i < LOOPS_PER_WORKER; ++i) {
        spin.lock();
        buffer += get_random_ascii_char();
        spin.unlock();
    }
}

void worker_with_spin_wait(int, std::string& buffer, SpinWait& sw) {
    for (int i = 0; i < LOOPS_PER_WORKER; ++i) {
        sw.lock();
        buffer += get_random_ascii_char();
        sw.unlock();
    }
}

void worker_with_semaphore(int, std::string& buffer, SyncSemaphore& sem) {
    for (int i = 0; i < LOOPS_PER_WORKER; ++i) {
        sem.acquire();
        buffer += get_random_ascii_char();
        sem.release();
    }
}

void worker_with_barrier(int, std::string& buffer, SyncBarrier& bar, std::mutex& mtx) {
    bar.wait();
    for (int i = 0; i < LOOPS_PER_WORKER; ++i) {
        std::lock_guard guard(mtx);
        buffer += get_random_ascii_char();
    }
}

void worker_with_monitor(int, std::string& buffer, SyncMonitor& mon) {
    for (int i = 0; i < LOOPS_PER_WORKER; ++i) {
        mon.lock();
        buffer += get_random_ascii_char();
        mon.unlock();
    }
}

void run_test(const std::string& label,
              std::function<void(std::vector<std::thread>&, std::string&)> test_logic) {
    std::cout << label << ":\n";
    std::string buffer;
    buffer.reserve(WORKER_COUNT * LOOPS_PER_WORKER);
    std::vector<std::thread> workers;
    TimeTracker timer;

    timer.start();
    test_logic(workers, buffer);
    for (auto& w : workers) w.join();
    timer.stop();

    auto actual = buffer.size();
    auto expected = WORKER_COUNT * LOOPS_PER_WORKER;
    std::cout << "    Время: " << timer.elapsed_milliseconds() << " мс\n";
    std::cout << "    Длина: " << actual << (actual != expected ? " (RACE!)" : " (OK)") << "\n\n";
}

void execute_concurrency_tests() {
    std::cout << "ЗАДАЧА 1: Тесты конкуренции\n";

    run_test("1. Без синхронизации", [](auto& workers, auto& buf) {
        for (int i = 0; i < WORKER_COUNT; ++i)
            workers.emplace_back(worker_no_lock, i, std::ref(buf));
    });

    std::mutex mtx;
    run_test("2. Mutex", [&](auto& workers, auto& buf) {
        for (int i = 0; i < WORKER_COUNT; ++i)
            workers.emplace_back(worker_with_mutex, i, std::ref(buf), std::ref(mtx));
    });

    SyncSemaphore sem(1);
    run_test("3. Semaphore", [&](auto& workers, auto& buf) {
        for (int i = 0; i < WORKER_COUNT; ++i)
            workers.emplace_back(worker_with_semaphore, i, std::ref(buf), std::ref(sem));
    });

    SyncBarrier bar(WORKER_COUNT);
    std::mutex bar_mtx;
    run_test("4. Barrier", [&](auto& workers, auto& buf) {
        for (int i = 0; i < WORKER_COUNT; ++i)
            workers.emplace_back(worker_with_barrier, i, std::ref(buf), std::ref(bar), std::ref(bar_mtx));
    });

    AtomicLock spin;
    run_test("5. SpinLock (Atomic)", [&](auto& workers, auto& buf) {
        for (int i = 0; i < WORKER_COUNT; ++i)
            workers.emplace_back(worker_with_atomic_lock, i, std::ref(buf), std::ref(spin));
    });

    SpinWait sw;
    run_test("6. SpinWait", [&](auto& workers, auto& buf) {
        for (int i = 0; i < WORKER_COUNT; ++i)
            workers.emplace_back(worker_with_spin_wait, i, std::ref(buf), std::ref(sw));
    });

    SyncMonitor mon;
    run_test("7. Monitor", [&](auto& workers, auto& buf) {
        for (int i = 0; i < WORKER_COUNT; ++i)
            workers.emplace_back(worker_with_monitor, i, std::ref(buf), std::ref(mon));
    });
}