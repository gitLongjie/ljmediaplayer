#ifndef src_network_channel_h_
#define src_network_channel_h_

#include <memory>
#include <string>

#include "src/lj_defined.h"
#include "src/task_queue.h"

namespace LJMP {
    namespace Network {
        class Socket;
        using SocketPtr = std::shared_ptr<Socket>;

        class Channel {
            disable_copy(Channel)

        public:
            static std::shared_ptr<Channel> create(const TaskQueuePtr& task_queue, const SocketPtr& s);

        public:
            virtual ~Channel();

        protected:
            Channel(const TaskQueuePtr& task_queue, const SocketPtr& s);

        private:
            TaskQueuePtr task_queue_;
            SocketPtr socket_;
        };

        using ChannelPtr = std::shared_ptr<Channel>;
    }
}

#endif // !src_network_channel_h_
