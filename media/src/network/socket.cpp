#include "src/network/socket.h"

#ifdef WIN32

#include <WinSock2.h>

#endif // WIN32


namespace LJMP {
    namespace Network {

        SocketPtr Socket::create(Model model) {
            struct Createor : public Socket {
                Createor(Model model) : Socket(model) {}
                ~Createor() override = default;
            };

            return std::make_shared<Createor>(model);
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

    }
}