#include "src/network/channel.h"

#include "src/core/log.h"
#include "src/network/socket.h"

#include "src/network/network_manager_std.h"
#include "src/media.h"

namespace LJMP {
    namespace Network {

        Channel::Ptr Channel::create(const TaskQueue::Ptr& task_queue, const SocketPtr& s) {
            struct Creator : public Channel {
                Creator(const TaskQueue::Ptr& task_queue, const SocketPtr& s)
                    : Channel(task_queue, s){ }
                ~Creator() override = default;
            };

            Ptr channel = std::make_shared<Creator>(task_queue, s);
            std::shared_ptr<NetworkManagerStd> network_manager = std::dynamic_pointer_cast<NetworkManagerStd>(
                Media::getInstance()->getNetworkManager());
            if (network_manager) {
                network_manager->updateChannel(channel);
            }
            return channel;
        }

        Channel::Channel(const TaskQueue::Ptr& task_queue, const SocketPtr& s)
            : TaskObject(task_queue, false)
            , socket_(s){
            LOGI("actor {}", (long long)this);
            
        }

        Channel::~Channel() {
            LOGI("dctor {}", (long long)this);
        }

        void Channel::setReadCallbackHandle(ReadCallbackHandle read_call_handle) {
            LOG_ENTER;

            WPtr wThis(shared_from_this());
            Task::Ptr task = createTask(std::bind(&Channel::doSetCallbackHandle, this, read_call_handle, wThis));
            invoke(task);
        }

        int Channel::read(DataBuffer::Ptr& data_buffer) {
            return socket_->read(data_buffer);
        }

        bool Channel::write(const DataBuffer::Ptr& data_buffer, WriteStatusCallback callback) {
            if (!socket_) {
                LOGE("socket is nullptr");
                return false;
            }
            WPtr wThis(shared_from_this());
            auto task = createTask(std::bind(&Channel::doWrite, this, data_buffer, callback, wThis));
            invoke(task);
            return true;
        }

        void Channel::disconnect() {
            LOG_ENTER;
            std::shared_ptr<NetworkManagerStd> network_manager = std::dynamic_pointer_cast<NetworkManagerStd>(
                Media::getInstance()->getNetworkManager());
            if (network_manager) {
                network_manager->removeChannel(std::dynamic_pointer_cast<Channel>(shared_from_this()));
            }
        }

        void Channel::handleRead() {
            if (!socket_) {
                disconnect();
                return;
            }

            if (read_callback_handle_) {
                read_callback_handle_();
            }

        }

        void Channel::doSetCallbackHandle(ReadCallbackHandle read_call_handle, WPtr wThis) {
            ObjectPtr::Ptr self(wThis.lock());
            if (!self) {
                LOGE("this object is destruct {}", (long long)this);
                return;
            }

            read_callback_handle_ = read_call_handle;
        }

        void Channel::doWrite(const DataBuffer::Ptr data_buffer, WriteStatusCallback callback, WPtr wThis) {
            ObjectPtr::Ptr self(wThis.lock());
            if (!self) {
                LOGE("this object is destruct");
                return;
            }

            if (!socket_) {
                LOGE("socket is nulptr");
                invokeCallback(callback, false);
                return;
            }

            int ret = socket_->write(data_buffer);
            invokeCallback(callback, data_buffer->getSize() == ret);
        }

    }
}