#include "src/network/socket.h"

#ifdef WIN32

#include <WinSock2.h>

#define SET_RCVTIMEO(tv,s)	int tv = s*1000

#endif // WIN32

#include "src/core/log.h"
#include "src/network/network_utils.h"

#include "src/network/tcp_socket.h"


namespace LJMP {
    namespace Network {

        Socket::Ptr Socket::create(Socket::Model model) {
            if (Socket::Model::TCP == model) {
                return TcpSocket::create();
            }
            return nullptr;
        }

        Socket::Ptr Socket::create(FD socket, Socket::Model model) {
            if (Socket::Model::TCP == model) {
                return TcpSocket::create(socket);
            }
            return nullptr;
        }

        Socket::Socket(Model model)
            : model_(model){
            if (Model::TCP == model_) {
                socket_ = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            }
            
        }

        Socket::Socket(FD socket, Model model)
            : model_(model)
            , socket_(socket){

        }

        Socket::~Socket() {
            close();
        }

        void Socket::close() {
            if (-1 != socket_) {
                ::closesocket(socket_);
            }
        }

        bool Socket::enableNoDelay(bool enable) {
            if (-1 == socket_) {
                return false;
            }

            int on = 1;
            if (!enable) {
                on = 0;
            }
            setsockopt(socket_, IPPROTO_TCP, TCP_NODELAY, (char*)&on, sizeof(on));
            return true;
        }

        bool Socket::enableBlock(bool enable) {
            if (-1 == socket_) {
                return false;
            }
#ifdef WIN32
            unsigned long ul = !enable;
            int ret = ioctlsocket(socket_, FIONBIO, (unsigned long*)&ul);
            if (SOCKET_ERROR == ret) {
                return false;
            }
            return true;
#endif
        }

        bool Socket::enableTimeout(bool enable, int time) {
            if (-1 == socket_) {
                return false;
            }

            SET_RCVTIMEO(tv, time);
            if (setsockopt(socket_, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(tv))) {
                return false;
            }
            return true;
        }

        bool Socket::doConnect(const struct sockaddr& service) {
            LOG_ENTER;

            if (::connect(socket_, &service, sizeof(sockaddr)) < 0) {
                LOGE("do connect failed, err = {}", NetworkUtils::getSocketError());
                return false;
            }

            struct sockaddr_in sa;
            int len = sizeof(sa);
            if (0 == getsockname(socket_, (struct sockaddr*)&sa, &len)) {
                std::string name = inet_ntoa(sa.sin_addr);
                short port = ntohs(sa.sin_port);

                seesion_ = name + "_" + std::to_string(port);
            }

            return true;
        }

    }
}