#include "src/network/tcp_socket.h"

#include "src/network/network_utils.h"
#include "src/log.h"


namespace LJMP {
    namespace Network {

        SocketPtr TcpSocket::create() {
            struct Creator : public TcpSocket {
                Creator() : TcpSocket() {}
                ~Creator() override {}
            };

            return std::make_shared<Creator>();
        }

        TcpSocket::TcpSocket()
            : Socket(Socket::Model::TCP) {

        }

        TcpSocket::~TcpSocket() {

        }

        bool TcpSocket::connect(const std::string& address, short port) {
            LOG_ENTER;

            struct sockaddr_in service;
            memset(&service, 0, sizeof(struct sockaddr_in));
            service.sin_family = AF_INET;

            if (!NetworkUtils::fillService(address.c_str(), port, &service)) {
                LOGE("fill service failed");
                return false;
            }

            if (!doConnect(*(struct sockaddr*)&service)) {
                return false;
            }
            return true;
        }

        int TcpSocket::read(unsigned char* buffer, unsigned int max_len) {
           return ::read(getSocket(), buffer, max_len);
        }

    }
}