#include "src/network/network_manager_std.h"

#include <vector>

#include "src/core/log.h"

#ifdef WIN32
#include "src/network/win/network_manager_win.h"
#endif // WIN32

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

        void NetworkManagerStd::updateChannel(const IChannel::Ptr& channel, IIOEvent::Event event) {
            LOG_ENTER;

            ObjectPtr::WPtr wThis(shared_from_this());
            auto task = createTask(std::bind(&NetworkManagerStd::doUpdateChannel, this, channel, event, wThis));
            invoke(task);
        }

        void NetworkManagerStd::removeChannel(const IChannel::Ptr& channel) {
            LOG_ENTER;

            ObjectPtr::WPtr wThis(shared_from_this());
            auto task = createTask(std::bind(&NetworkManagerStd::doRemoveChannel, this, channel, wThis));
            invoke(task);
        }

		void NetworkManagerStd::addConnectChannel(const IChannel::Ptr& channel)
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

        void NetworkManagerStd::doUpdateChannel(const IChannel::Ptr& channel, IIOEvent::Event event, ObjectPtr::WPtr wThis) {
            LOG_ENTER;

            ObjectPtr::Ptr self(wThis.lock());
            if (!self) {
                LOGE("this object is desturcted {}", (long long)this);
                return;
            }

            io_event_->updateChannel(channel, event);
        }

        void NetworkManagerStd::doRemoveChannel(const IChannel::Ptr& channel, ObjectPtr::WPtr wThis) {
            LOG_ENTER;

            ObjectPtr::Ptr self(wThis.lock());
            if (!self) {
                LOGE("this object is desturcted {}", (long long)this);
                return;
            }
            io_event_->updateChannel(channel, IIOEvent::Event::E_Remove);
        }

		void NetworkManagerStd::doAddConnectChannel(const IChannel::Ptr& channel, ObjectPtr::WPtr wThis) {
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

            io_event_->updateChannel(channel, IIOEvent::Event::E_WriteEable);
		}
    }
}