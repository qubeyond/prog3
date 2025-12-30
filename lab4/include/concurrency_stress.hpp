#pragma once

#include <atomic>
#include <mutex>
#include <string>
#include <vector>
#include "thread_locks.hpp"

inline constexpr int WORKER_COUNT = 10;
inline constexpr int LOOPS_PER_WORKER = 1000;

char get_random_ascii_char();

void worker_no_lock(int worker_id, std::string& buffer);
void worker_with_mutex(int worker_id, std::string& buffer, std::mutex& lock);
void worker_with_atomic_lock(int worker_id, std::string& buffer, AtomicLock& spin);
void worker_with_spin_wait(int worker_id, std::string& buffer, SpinWait& sw);
void worker_with_semaphore(int worker_id, std::string& buffer, SyncSemaphore& sem);
void worker_with_barrier(int worker_id, std::string& buffer, SyncBarrier& bar, std::mutex& mtx);
void worker_with_monitor(int worker_id, std::string& buffer, SyncMonitor& mon);

void execute_concurrency_tests();