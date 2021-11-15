#ifndef src_network_socket_h_
#define src_network_socket_h_

#include <memory>

#include "src/lj_defined.h"

#ifdef WIN32

#include <WinSock2.h>

using socket_t = SOCKET;

#endif // WIN32

namespace LJMP {
    namespace Network {
        class Socket {
            disable_copy(Socket)

        public:
            enum class Model {
                Unknow,
                TCP,
                UDP
            };
            static std::shared_ptr<Socket> create(Model model);

        public:
            virtual ~Socket();

            bool isTcp() const { return model_ == Model::TCP; }
            void close();

        protected:
            Socket(Model model);

        private:
            Model model_ = Model::Unknow;
            socket_t socket_ = -1;
        };

        using SocketPtr = std::shared_ptr<Socket>;
    }
}

#endif // !src_network_socket_h_

