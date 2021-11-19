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

        class Channel : public std::enable_shared_from_this<Channel> {
            disable_copy(Channel)

        public:
            using ChannelPtr = std::shared_ptr<Channel>;
            using ChannelWPtr = std::weak_ptr<Channel>;

            using ReadCallbackHandle = std::function<void(const SocketPtr& sc)>;

            static std::shared_ptr<Channel> create(const TaskQueuePtr& task_queue, const SocketPtr& s);

        public:
            virtual ~Channel();

            void setReadCallbackHandle(ReadCallbackHandle read_call_handle);

            void disconnect();

            SocketPtr getSocket() const { return socket_; }


        protected:
            Channel(const TaskQueuePtr& task_queue, const SocketPtr& s);

            void handleRead();

            void doSetCallbackHandle(ReadCallbackHandle read_call_handle, ChannelWPtr wThis);

            void invoke(const TaskPtr task);

        private:
            TaskQueuePtr task_queue_;
            SocketPtr socket_;

            ReadCallbackHandle read_callback_handle_;

            friend class NetworkManagerStd;
        };
    }
}

#endif // !src_network_channel_h_
