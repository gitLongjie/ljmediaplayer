#include "src/network/tcp_socket.h"

#include "src/network/network_utils.h"
#include "src/core/log.h"


namespace LJMP {
    namespace Network {

        Socket::Ptr TcpSocket::create() {
            struct Creator : public TcpSocket {
                Creator() : TcpSocket() {}
                ~Creator() override {}
            };

            return std::make_shared<Creator>();
        }

        Socket::Ptr TcpSocket::create(FD socket) {
            struct Creator : public TcpSocket {
                explicit Creator(FD socket) : TcpSocket(socket) {}
                ~Creator() override {}
            };

            return std::make_shared<Creator>(socket);
        }

        TcpSocket::TcpSocket()
            : Socket(Socket::Model::TCP) {

        }

        TcpSocket::TcpSocket(FD socket)
            : Socket(socket, Socket::Model::TCP) {

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

        int TcpSocket::read(unsigned char* buffer, unsigned int size) {
            FD sc = getSocket();
            if (-1 == sc) {
                LOGE("cont write data sc={}", sc);
                return 0;
            }

            char* p = reinterpret_cast<char*>(buffer);
            int len = size;
            int readed = 0;
            do 
            {
                int ret = ::recv(sc, p, len, 0);
                if (ret == -1) {
                    int err = 0;//  getSockError();
                   LOGE("recv error, err={}", err);
                   return 0;
                }
                readed += ret;
                len -= ret;
            } while (len > 0);
          
            return readed;
        }

        int TcpSocket::write(const unsigned char* buffer, unsigned int size) {
            FD sc = getSocket();
            if (-1 == sc) {
                LOGE("cont write data sc={}", sc);
                return 0;
            }
            
            const char* p = reinterpret_cast<const char*>(buffer);
            int len = size;
            int sended = 0;
            do {
                int ret = ::send(sc, p, len, 0);
                len = len - ret;
                sended += ret;
                p = p + ret;
            } while (len > 0);

            return sended;
        }

    }
}