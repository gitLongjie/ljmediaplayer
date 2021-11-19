#include "src/network/network_manager_std.h"

#include "src/log.h"

#ifdef WIN32
#include "src/network/win/network_manager_win.h"
#endif // WIN32

#include "src/network/channel.h"
#include "src/network/socket.h"


namespace LJMP {
    namespace Network {
        
        NetworkManagerPtr NetworkManagerStd::create(TaskQueuePtr task_queue) {
#ifdef WIN32
            return NetworkManagerWin::create(task_queue);
#endif // WIN32

            struct Creator : public NetworkManagerStd {
                Creator(TaskQueuePtr task_queue) : NetworkManagerStd(task_queue) {}
                ~Creator() override = default;
            };
            return std::make_shared<Creator>(task_queue);
        }

        NetworkManagerStd::NetworkManagerStd(TaskQueuePtr task_queue) : io_task_queue_(task_queue) {
            LOGI("actor {}", (long long)this);
        }

        NetworkManagerStd::~NetworkManagerStd() {
            LOGI("dctor {}", (long long)this);
        }

        bool NetworkManagerStd::initialize() {
            LOG_ENTER;

            NetworkManagerWPtr wThis(shared_from_this());
            auto task = createTask(std::bind(&NetworkManagerStd::doInitialize, this, wThis));
            invoke(task);
            return true;
        }

        void NetworkManagerStd::uninitialize() {
            LOG_ENTER;

            NetworkManagerWPtr wThis(shared_from_this());
            auto task = createTask(std::bind(&NetworkManagerStd::doUninitialize, this, wThis));
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

        void NetworkManagerStd::updateChannel(const std::shared_ptr<Channel>& channel) {
            LOG_ENTER;

            NetworkManagerWPtr wThis(shared_from_this());
            if (io_task_queue_->isCurrentThread()) {
                doUpdateChannel(channel, wThis);
            }
            else {
                auto task = createTask(std::bind(&NetworkManagerStd::doUpdateChannel, this, channel, wThis));
                invoke(task);
            }
        }

        void NetworkManagerStd::removeChannel(const std::shared_ptr<Channel>& channel) {
            LOG_ENTER;

            NetworkManagerWPtr wThis(shared_from_this());
            if (io_task_queue_->isCurrentThread()) {
                doRemoveChannel(channel, wThis);
            }
            else {
                auto task = createTask(std::bind(&NetworkManagerStd::doRemoveChannel, this, channel, wThis));
                invoke(task);
            }

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

        void NetworkManagerStd::doUpdateChannel(const std::shared_ptr<Channel>& channel, NetworkManagerWPtr wThis) {
            LOG_ENTER;

            NetworkManagerPtr self(wThis.lock());
            if (!self) {
                LOGE("this object is desturcted {}", (long long)this);
                return;
            }

            if (!channel) {
                LOGE("channel is nullptr");
                return;
            }

            SocketPtr sc = channel->getSocket();
            if (!sc) {
                LOGE("channel is nullptr");
                return;
            }

            channels_[sc->getSocket()] = channel;

        }

        void NetworkManagerStd::doRemoveChannel(const std::shared_ptr<Channel>& channel, NetworkManagerWPtr wThis) {
            LOG_ENTER;

            NetworkManagerPtr self(wThis.lock());
            if (!self) {
                LOGE("this object is desturcted {}", (long long)this);
                return;
            }

            if (!channel) {
                LOGE("channel is nullptr");
                return;
            }

            SocketPtr sc = channel->getSocket();
            if (!sc) {
                LOGE("channel is nullptr");
                return;
            }

            sc->close();
            channels_.erase(sc->getSocket());
        }

        void NetworkManagerStd::select() {
            //int t
        }

        void NetworkManagerStd::doSelect(NetworkManagerWPtr wThis) {
            NetworkManagerPtr self(wThis.lock());
            if (!self) {
                LOGE("this object is destruct {}", (long long)this);
                return;
            }

            fd_set reads;
            FD_ZERO(&reads);

            int count = static_cast<int>(channels_.size()) + 1;
            timeval timeout = { 0, 1000 };
            int ret = ::select(count, &reads, nullptr, nullptr, &timeout);
            if (ret < 0) {
                LOGE("select end");
                return;
            }

            for (const auto& item : channels_) {
                if (FD_ISSET(item.first, &reads)) {
                    item.second->handleRead();
                }
            }
        }

    }
}