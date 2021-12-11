#ifndef src_network_tcp_socket_h_
#define src_network_tcp_socket_h_

#include "src/network/socket.h"

namespace LJMP {
    namespace Network {
        class TcpSocket : public Socket {
            disable_copy(TcpSocket)

        public:
            static std::shared_ptr<Socket> create();

        public:
            ~TcpSocket() override;

            bool connect(const std::string& address, short port) override;
            int read(DataBuffer::Ptr& buffer) override;
            int write(const DataBuffer::Ptr& buffer) override;

        protected:
            TcpSocket();
        };
    }
}

#endif // !src_network_tcp_socket_h_
