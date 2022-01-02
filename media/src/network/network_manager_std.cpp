#include "src/network/network_manager_std.h"

#include <vector>

#include "src/core/log.h"

#ifdef WIN32
#include "src/network/win/network_manager_win.h"
#endif // WIN32

#include "src/network/channel.h"
#include "src/network/socket.h"


namespace LJMP {
    namespace Network {
        
        INetworkManager::Ptr NetworkManagerStd::create(const IIOEvent::Ptr& io_event, const TaskQueue::Ptr& task_queue) {
#ifdef WIN32
            return NetworkManagerWin::create(io_event, task_queue);
#endif // WIN32
            
            return createPtr<NetworkManagerStd>(io_event, task_queue);
        }

        NetworkManagerStd::NetworkManagerStd(const IIOEvent::Ptr& io_event, const TaskQueue::Ptr& task_queue)
            : TaskObject(task_queue, false)
            , io_event_(io_event) {
            LOG_CREATER;
        }

        NetworkManagerStd::~NetworkManagerStd() {
            LOG_DESTRUCT;
        }

        bool NetworkManagerStd::initialize() {
            LOG_ENTER;

            ObjectPtr::WPtr wThis(shared_from_this());
            auto task = createTask(std::bind(&NetworkManagerStd::doInitialize, this, wThis));
            invoke(task);
            return true;
        }

        void NetworkManagerStd::uninitialize() {
            LOG_ENTER;

            ObjectPtr::WPtr wThis(shared_from_this());
            auto task = createTask(std::bind(&NetworkManagerStd::doUninitialize, this, wThis));
            invoke(task);

            spin_lock_.lock();
        }

        void NetworkManagerStd::updateChannel(const std::shared_ptr<Channel>& channel) {
            LOG_ENTER;

            ObjectPtr::WPtr wThis(shared_from_this());
            auto task = createTask(std::bind(&NetworkManagerStd::doUpdateChannel, this, channel, wThis));
            invoke(task);
        }

        void NetworkManagerStd::removeChannel(const std::shared_ptr<Channel>& channel) {
            LOG_ENTER;

            ObjectPtr::WPtr wThis(shared_from_this());
            auto task = createTask(std::bind(&NetworkManagerStd::doRemoveChannel, this, channel, wThis));
            invoke(task);
        }

		void NetworkManagerStd::addConnectChannel(const std::shared_ptr<Channel>& channel)
		{
			LOG_ENTER;

            ObjectPtr::WPtr wThis(shared_from_this());
			auto task = createTask(std::bind(&NetworkManagerStd::doAddConnectChannel, this, channel, wThis));
			invoke(task);
		}

		void NetworkManagerStd::doInitialize(ObjectPtr::WPtr wThis) {
            LOG_ENTER;
            ObjectPtr::Ptr self(wThis.lock());
            if (!self) {
                LOGE("self is nullptr");
                return;
            }

            if (io_event_) {
                io_event_->initialize();
            }
            else {
                LOGD("io event is nullptr");
            }

            stop_ = false;
        }

        void NetworkManagerStd::doUninitialize(ObjectPtr::WPtr wThis) {
            LOG_ENTER;
            ObjectPtr::Ptr self(wThis.lock());
            if (!self) {
                LOGE("self is nullptr");
                return;
            }

            if (io_event_) {
                io_event_->uninitialize();
            }
            else {
                LOGD("io event is nullptr");
            }
            stop_ = true;

            spin_lock_.unlock();
        }

        void NetworkManagerStd::doUpdateChannel(const std::shared_ptr<Channel>& channel, ObjectPtr::WPtr wThis) {
            LOG_ENTER;

            ObjectPtr::Ptr self(wThis.lock());
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

        void NetworkManagerStd::doRemoveChannel(const std::shared_ptr<Channel>& channel, ObjectPtr::WPtr wThis) {
            LOG_ENTER;

            ObjectPtr::Ptr self(wThis.lock());
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

		void NetworkManagerStd::doAddConnectChannel(const std::shared_ptr<Channel>& channel, ObjectPtr::WPtr wThis) {
			LOG_ENTER;

            ObjectPtr::Ptr self(wThis.lock());
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
    }
}