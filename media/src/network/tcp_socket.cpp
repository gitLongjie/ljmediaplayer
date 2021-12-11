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

        int TcpSocket::read(DataBuffer::Ptr& buffer) {
            socket_t sc = getSocket();
            if (-1 == sc) {
                LOGE("cont write data sc={}", sc);
                return 0;
            }

            char* p = buffer->getData();
            int len = buffer->getSize();
            int readed = 0;
            do 
            {
                int ret = ::recv(sc, p, len, 0);
                if (ret == -1) {
                   int err = getSockError();
                   LOGE("recv error, err={}", err);
                   return 0;
                }
                readed += ret;
                len -= ret;
            } while (len > 0);
          
            return readed;
        }

        int TcpSocket::write(const DataBuffer::Ptr& buffer) {
            socket_t sc = getSocket();
            if (-1 == sc) {
                LOGE("cont write data sc={}", sc);
                return 0;
            }
            
            const char* p = buffer->getData();
            int len = buffer->getSize();
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