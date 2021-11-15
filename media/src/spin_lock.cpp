#include "src/spin_lock.h"

#include <thread>
#include <chrono>

namespace LJMP {

    SpinLock::SpinLock() {

    }

    SpinLock::~SpinLock() {
        lock_.store(false);
    }

    void SpinLock::lock() {
        lock_.store(true);
        while (lock_.load()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }

    void SpinLock::unlock() {
        lock_.store(false);
    }

}