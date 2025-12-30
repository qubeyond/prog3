#pragma once

#include <atomic>
#include <condition_variable>
#include <mutex>

class AtomicLock {
public:
    void lock() noexcept;
    void unlock() noexcept;
private:
    std::atomic_flag flag_state = ATOMIC_FLAG_INIT;
};

class SpinWait {
public:
    void lock() noexcept;
    void unlock() noexcept;
private:
    std::atomic_flag flag_state = ATOMIC_FLAG_INIT;
};

class SyncSemaphore {
public:
    explicit SyncSemaphore(int count) noexcept;
    void acquire();
    void release();
private:
    std::mutex mtx;
    std::condition_variable cv;
    int counter;
};

class SyncBarrier {
public:
    explicit SyncBarrier(int count) noexcept;
    void wait();
private:
    std::mutex mtx;
    std::condition_variable cv;
    int threshold;
    int count;
    int generation;
};

class SyncMonitor {
public:
    SyncMonitor() noexcept;
    void lock();
    void unlock();
    void wait();
    void notify_all();
    void reset();
private:
    std::mutex protection_lock;
    std::condition_variable cond_var;
    bool is_ready;
};