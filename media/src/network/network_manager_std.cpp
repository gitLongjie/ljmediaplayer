#include "src/network/network_manager_std.h"

#include "src/log.h"

#ifdef WIN32
#include "src/network/win/network_manager_win.h"
#endif // WIN32


namespace LJMP {
    namespace Network {
        
        NetworkManagerPtr NetworkManagerStd::create(TaskQueuePtr taskQueue) {
#ifdef WIN32
            return NetworkManagerWin::create(taskQueue);
#endif // WIN32

            struct Creator : public NetworkManagerStd {
                Creator(TaskQueuePtr taskQueue) : NetworkManagerStd(taskQueue) {}
                ~Creator() override = default;
            };
            return std::make_shared<Creator>(taskQueue);
        }

        NetworkManagerStd::NetworkManagerStd(TaskQueuePtr taskQueue) : io_task_queue_(taskQueue) {
            LOGI("actor {}", (long long)this);
        }

        NetworkManagerStd::~NetworkManagerStd() {
            LOGI("dctor {}", (long long)this);
        }

        bool NetworkManagerStd::initialize() {
            LOG_ENTER;

            auto task = createTask(std::bind(&NetworkManagerStd::doInitialize, this, shared_from_this()));
            invoke(task);
            return true;
        }

        void NetworkManagerStd::uninitialize() {
            LOG_ENTER;
            auto task = createTask(std::bind(&NetworkManagerStd::doUninitialize, this, shared_from_this()));
            invoke(task);

            spin_lock_.lock();
        }

        void NetworkManagerStd::invoke(const TaskPtr task) {
            if (!task || !io_task_queue_) {
                LOGE("task is nullptr or io task queeu is nullptr, task={}, queue={}",
                    nullptr == task, nullptr == io_task_queue_);
                return;
            }
            io_task_queue_->push(task);
        }

        void NetworkManagerStd::invoke(const TaskPtr task, uint16_t delay) {
            if (!task || !io_task_queue_) {
                LOGE("task is nullptr or io task queeu is nullptr, task={}, queue={}",
                    nullptr == task, nullptr == io_task_queue_);
                return;
            }
            io_task_queue_->push(task, delay);
        }

        void NetworkManagerStd::doInitialize(NetworkManagerWPtr wThis) {
            LOG_ENTER;
            NetworkManagerPtr self = wThis.lock();
            if (!self) {
                LOGE("self is nullptr");
            }
        }

        void NetworkManagerStd::doUninitialize(NetworkManagerWPtr wThis) {
            LOG_ENTER;
            NetworkManagerPtr self = wThis.lock();
            if (!self) {
                LOGE("self is nullptr");
            }

            spin_lock_.unlock();
        }

    }
}