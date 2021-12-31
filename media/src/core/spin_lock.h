#ifndef src_core_spin_lock_h_
#define src_core_spin_lock_h_

#include <atomic>

#include "src/lj_defined.h"

namespace LJMP {
    class SpinLock {
        disable_copy(SpinLock)

    public:
        SpinLock();
        ~SpinLock();

        void lock();
        void unlock();

    private:
        std::atomic_bool lock_ = false;
    };
}

#endif // !src_core_spin_lock_h_

