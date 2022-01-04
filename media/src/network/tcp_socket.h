#ifndef src_network_tcp_socket_h_
#define src_network_tcp_socket_h_

#include "src/network/socket.h"

namespace LJMP {
    namespace Network {
        class TcpSocket : public Socket {
            disable_copy(TcpSocket)

        public:
            static Socket::Ptr create();
            static Socket::Ptr create(FD socket);

        public:
            ~TcpSocket() override;

            bool connect(const std::string& address, short port) override;

            int read(unsigned char* buffer, unsigned int size) override;
            int write(const unsigned char* buffer, unsigned int size) override;

        protected:
            TcpSocket();
            TcpSocket(FD socket);
        };
    }
}

#endif // !src_network_tcp_socket_h_
