#ifndef src_network_channel_h_
#define src_network_channel_h_

#include <memory>
#include <string>

#include "src/lj_defined.h"
#include "src/task_object.h"
#include "src/data_buffer.h"

namespace LJMP {
    namespace Network {
        class Socket;
        using SocketPtr = std::shared_ptr<Socket>;

        class Channel : public TaskObject {
            disable_copy(Channel)

        public:
            using Ptr = std::shared_ptr<Channel>;

            using ReadCallbackHandle = std::function<void()>;
            using WriteStatusCallback = std::function<void(bool)>;

            static Channel::Ptr create(const TaskQueue::Ptr& task_queue, const SocketPtr& s);

        public:
            virtual ~Channel();

            void setReadCallbackHandle(ReadCallbackHandle read_call_handle);

            int read(DataBuffer::Ptr& data_buffer);
            bool write(const DataBuffer::Ptr& data_buffer, WriteStatusCallback callback);

            void disconnect();

            SocketPtr getSocket() const { return socket_; }


        protected:
            Channel(const TaskQueue::Ptr& task_queue, const SocketPtr& s);

            void handleRead();

            void doSetCallbackHandle(ReadCallbackHandle read_call_handle, WPtr wThis);
            void doWrite(const DataBuffer::Ptr data_buffer, WriteStatusCallback callback, WPtr wThis);

        private:
            SocketPtr socket_;

            ReadCallbackHandle read_callback_handle_;

            friend class NetworkManagerStd;
        };
    }
}

#endif // !src_network_channel_h_
