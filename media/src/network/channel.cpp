#include "src/network/channel.h"

#include "src/log.h"
#include "src/network/socket.h"

#include "src/network/network_manager_std.h"
#include "src/media.h"

namespace LJMP {
    namespace Network {

        ChannelPtr Channel::create(const TaskQueuePtr& task_queue, const SocketPtr& s) {
            struct Creator : public Channel {
                Creator(const TaskQueuePtr& task_queue, const SocketPtr& s)
                    : Channel(task_queue, s){ }
                ~Creator() override = default;
            };

            ChannelPtr channel = std::make_shared<Creator>(task_queue, s);
            NetworkManagerStdPtr network_manager = std::dynamic_pointer_cast<NetworkManagerStd>(
                Media::getInstance()->getNetworkManager());
            if (network_manager) {
            //    network_manager->
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

    }
}