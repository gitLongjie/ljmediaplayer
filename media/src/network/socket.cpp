#include "src/network/socket.h"

#ifdef WIN32

#include <WinSock2.h>

#endif // WIN32

#include "src/log.h"
#include "src/network/network_utils.h"

#include "src/network/tcp_socket.h"


namespace LJMP {
    namespace Network {

        SocketPtr Socket::create(Socket::Model model) {
            if (Socket::Model::TCP == model) {
                return TcpSocket::create();
            }
            return nullptr;
        }

        Socket::Socket(Model model) 
            : model_(model){
            if (Model::TCP == model_) {
                socket_ = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            }
            
        }

        Socket::~Socket() {
            close();
        }

        void Socket::close() {
            if (-1 != socket_) {
                ::closesocket(socket_);
            }
        }

        bool Socket::doConnect(const struct sockaddr& service) {
            LOG_ENTER;

            if (::connect(socket_, &service, sizeof(sockaddr)) < 0) {
                LOGE("do connect failed, err = {}", NetworkUtils::getSocketError());
                return false;
            }

            return true;
        }

    }
}