#ifndef src_network_channel_h_
#define src_network_channel_h_

#include <memory>
#include <string>

#include "src/lj_defined.h"
#include "src/task_queue.h"
#include "src/data_buffer.h"

namespace LJMP {
    namespace Network {
        class Socket;
        using SocketPtr = std::shared_ptr<Socket>;

        class Channel : public std::enable_shared_from_this<Channel> {
            disable_copy(Channel)

        public:
            using Ptr = std::shared_ptr<Channel>;
            using WPtr = std::weak_ptr<Channel>;

            using ReadCallbackHandle = std::function<void(const Ptr& self)>;
            using WriteStatusCallback = std::function<void(bool)>;

            static std::shared_ptr<Channel> create(const TaskQueuePtr& task_queue, const SocketPtr& s);

        public:
            virtual ~Channel();

            void setReadCallbackHandle(ReadCallbackHandle read_call_handle);

            int read(DataBuffer::Ptr& data_buffer);
            bool write(const DataBuffer::Ptr& data_buffer, WriteStatusCallback callback);

            void disconnect();

            SocketPtr getSocket() const { return socket_; }


        protected:
            Channel(const TaskQueuePtr& task_queue, const SocketPtr& s);

            void handleRead();

            void doSetCallbackHandle(ReadCallbackHandle read_call_handle, WPtr wThis);
            void doWrite(const DataBuffer::Ptr data_buffer, WriteStatusCallback callback, WPtr wThis);

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
