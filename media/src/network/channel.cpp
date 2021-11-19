#include "src/network/channel.h"

#include "src/log.h"
#include "src/network/socket.h"

#include "src/network/network_manager_std.h"
#include "src/media.h"

namespace LJMP {
    namespace Network {

        Channel::ChannelPtr Channel::create(const TaskQueuePtr& task_queue, const SocketPtr& s) {
            struct Creator : public Channel {
                Creator(const TaskQueuePtr& task_queue, const SocketPtr& s)
                    : Channel(task_queue, s){ }
                ~Creator() override = default;
            };

            ChannelPtr channel = std::make_shared<Creator>(task_queue, s);
            NetworkManagerStdPtr network_manager = std::dynamic_pointer_cast<NetworkManagerStd>(
                Media::getInstance()->getNetworkManager());
            if (network_manager) {
                network_manager->updateChannel(channel);
            }
            return channel;
        }

        Channel::Channel(const TaskQueuePtr& task_queue, const SocketPtr& s)
            : task_queue_(task_queue)
            , socket_(s){
            LOGI("actor {}", (long long)this);
            
        }

        Channel::~Channel() {
            LOGI("dctor {}", (long long)this);
        }

        void Channel::setReadCallbackHandle(ReadCallbackHandle read_call_handle) {
            LOG_ENTER;

            ChannelWPtr wThis(shared_from_this());
            TaskPtr task = createTask(std::bind(&Channel::doSetCallbackHandle, this, read_call_handle, wThis));
            invoke(task);
        }

        void Channel::disconnect() {
            LOG_ENTER;
            NetworkManagerStdPtr network_manager = std::dynamic_pointer_cast<NetworkManagerStd>(
                Media::getInstance()->getNetworkManager());
            if (network_manager) {
                network_manager->removeChannel(shared_from_this());
            }
        }

        void Channel::handleRead() {
            if (!socket_) {
                disconnect();
                return;
            }

            if (read_callback_handle_) {
                read_callback_handle_(socket_);
            }

        }

        void Channel::doSetCallbackHandle(ReadCallbackHandle read_call_handle, ChannelWPtr wThis) {
            ChannelPtr self(wThis.lock());
            if (!self) {
                LOGE("this object is destruct {}", (long long)this);
                return;
            }

            read_callback_handle_ = read_call_handle;
        }

        void Channel::invoke(const TaskPtr task) {
            if (!task_queue_ || task_queue_->isCurrentThread()) {
                task->execute();
                return;
            }

            task_queue_->push(task);
        }

    }
}