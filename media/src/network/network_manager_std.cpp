#include "src/network/network_manager_std.h"

#include <vector>

#include "src/log.h"

#ifdef WIN32
#include "src/network/win/network_manager_win.h"
#endif // WIN32

#include "src/network/channel.h"
#include "src/network/socket.h"


namespace LJMP {
    namespace Network {
        
        NetworkManager::Ptr NetworkManagerStd::create(const TaskQueue::Ptr& task_queue) {
#ifdef WIN32
            return NetworkManagerWin::create(task_queue);
#endif // WIN32

            struct Creator : public NetworkManagerStd {
                Creator(const TaskQueue::Ptr& task_queue) : NetworkManagerStd(task_queue) {}
                ~Creator() override = default;
            };
            return std::make_shared<Creator>(task_queue);
        }

        NetworkManagerStd::NetworkManagerStd(const TaskQueue::Ptr& task_queue) : NetworkManager(task_queue) {
            LOG_CREATER;
        }

        NetworkManagerStd::~NetworkManagerStd() {
            LOG_DESTRUCT;
        }

        bool NetworkManagerStd::initialize() {
            LOG_ENTER;

            WPtr wThis(shared_from_this());
            auto task = createTask(std::bind(&NetworkManagerStd::doInitialize, this, wThis));
            invoke(task);
            return true;
        }

        void NetworkManagerStd::uninitialize() {
            LOG_ENTER;

            WPtr wThis(shared_from_this());
            auto task = createTask(std::bind(&NetworkManagerStd::doUninitialize, this, wThis));
            invoke(task);

            spin_lock_.lock();
        }

        void NetworkManagerStd::updateChannel(const std::shared_ptr<Channel>& channel) {
            LOG_ENTER;

            WPtr wThis(shared_from_this());
            auto task = createTask(std::bind(&NetworkManagerStd::doUpdateChannel, this, channel, wThis));
            invoke(task);
        }

        void NetworkManagerStd::removeChannel(const std::shared_ptr<Channel>& channel) {
            LOG_ENTER;

            WPtr wThis(shared_from_this());
            auto task = createTask(std::bind(&NetworkManagerStd::doRemoveChannel, this, channel, wThis));
            invoke(task);
        }

		void NetworkManagerStd::addConnectChannel(const std::shared_ptr<Channel>& channel)
		{
			LOG_ENTER;

			WPtr wThis(shared_from_this());
			auto task = createTask(std::bind(&NetworkManagerStd::doAddConnectChannel, this, channel, wThis));
			invoke(task);
		}

		void NetworkManagerStd::doInitialize(WPtr wThis) {
            LOG_ENTER;
            TaskObject::Ptr self(wThis.lock());
            if (!self) {
                LOGE("self is nullptr");
                return;
            }

            stop_ = false;
            select(0);
        }

        void NetworkManagerStd::doUninitialize(WPtr wThis) {
            LOG_ENTER;
            TaskObject::Ptr self(wThis.lock());
            if (!self) {
                LOGE("self is nullptr");
                return;
            }

            stop_ = true;

            spin_lock_.unlock();
        }

        void NetworkManagerStd::doUpdateChannel(const std::shared_ptr<Channel>& channel, WPtr wThis) {
            LOG_ENTER;

            TaskObject::Ptr self(wThis.lock());
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

        void NetworkManagerStd::doRemoveChannel(const std::shared_ptr<Channel>& channel, WPtr wThis) {
            LOG_ENTER;

            TaskObject::Ptr self(wThis.lock());
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

		void NetworkManagerStd::doAddConnectChannel(const std::shared_ptr<Channel>& channel, WPtr wThis) {
			LOG_ENTER;

			TaskObject::Ptr self(wThis.lock());
			if (!self) {
                LOG_DESTRUCT;
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
			connect_channels_[sc->getSocket()] = channel;
		}

		void NetworkManagerStd::select(unsigned long long dely) {
            if (stop_) {
                LOGI("stop this select");
                return;
            }
            WPtr wThis(shared_from_this());
            auto task = createTask(std::bind(&NetworkManagerStd::doSelect, this, wThis));
            invoke(task, dely);
        }

        void NetworkManagerStd::doSelect(WPtr wThis) {
            TaskObject::Ptr self(wThis.lock());
            if (!self) {
                LOGE("this object is destruct {}", (long long)this);
                return;
            }

            if (channels_.empty()) {
                select(10);
                return;
            }

            int count = static_cast<int>(channels_.size()) + 1;

            fd_set reads;
            FD_ZERO(&reads);

            for (auto& item : channels_) {
                FD_SET(item.first, &reads);
            }
           
            struct timeval timeout;
            timeout.tv_sec = 3 * 60;
            timeout.tv_usec = 100; // { 3 * 60, 0 };

            int ret = ::select(count, &reads, nullptr, nullptr, &timeout);
            if (ret < 0) {
                LOGE("select end");
                return;
            }
            else if (0 == ret) {
                select(10);
                return;
            }

            std::vector<std::shared_ptr<Channel> > temp;
            for (const auto& item : channels_) {
                if (FD_ISSET(item.first, &reads)) {
                    temp.emplace_back(item.second);
                }
            }

            for (const auto& item : temp) {
                item->handleRead();
            }

            select(0);
        }

    }
}