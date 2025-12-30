#include "thread_locks.hpp"
#include <thread>

void AtomicLock::lock() noexcept {
    while (flag_state.test_and_set(std::memory_order_acquire));
}

void AtomicLock::unlock() noexcept {
    flag_state.clear(std::memory_order_release);
}

void SpinWait::lock() noexcept {
    int iterations = 0;
    while (flag_state.test_and_set(std::memory_order_acquire)) {
        iterations++;
        if (iterations > 100) {
            std::this_thread::yield();
        }
    }
}

void SpinWait::unlock() noexcept {
    flag_state.clear(std::memory_order_release);
}

SyncSemaphore::SyncSemaphore(int count) noexcept : counter(count) {}

void SyncSemaphore::acquire() {
    std::unique_lock lock(mtx);
    cv.wait(lock, [this] { return counter > 0; });
    counter--;
}

void SyncSemaphore::release() {
    std::unique_lock lock(mtx);
    counter++;
    cv.notify_one();
}

SyncBarrier::SyncBarrier(int count) noexcept : threshold(count), count(count), generation(0) {}

void SyncBarrier::wait() {
    std::unique_lock lock(mtx);
    int gen = generation;
    if (--count == 0) {
        generation++;
        count = threshold;
        cv.notify_all();
    } else {
        cv.wait(lock, [this, gen] { return gen != generation; });
    }
}

SyncMonitor::SyncMonitor() noexcept : is_ready(false) {}

void SyncMonitor::lock() {
    protection_lock.lock();
}

void SyncMonitor::unlock() {
    protection_lock.unlock();
}

void SyncMonitor::wait() {
    std::unique_lock lk(protection_lock);
    cond_var.wait(lk, [this] { return is_ready; });
}

void SyncMonitor::notify_all() {
    {
        std::lock_guard guard(protection_lock);
        is_ready = true;
    }
    cond_var.notify_all();
}

void SyncMonitor::reset() {
    std::lock_guard guard(protection_lock);
    is_ready = false;
}