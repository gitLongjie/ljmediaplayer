#ifndef src_network_socket_h_
#define src_network_socket_h_

#include <memory>

#include "src/lj_defined.h"
#include "src/core/object_sptr.h"
#include "src/kernel/read_writer.h"
#include "src/network/lj_network_define.h"

namespace LJMP {
    namespace Network {
        class Socket : public ObjectPtr, public IReadWriter {
        public:
            enum class Model {
                Unknow,
                TCP,
                UDP
            };

            using Ptr = std::shared_ptr<Socket>;

            static Ptr create(Socket::Model model);
            static Ptr create(socket_t socket, Socket::Model model);
        public:
            virtual ~Socket();

            virtual bool connect(const std::string& address, short port) = 0;

            bool isTcp() const { return model_ == Model::TCP; }
            void close();

            bool enableNoDelay(bool enable);
            bool enableBlock(bool enable);
            bool enableTimeout(bool enable, int time);

            const std::string& getSessionName() const { return seesion_; }
            socket_t getSocket() const { return socket_; }

        protected:
            explicit Socket(Model model);
            Socket(socket_t socket, Model model);

            bool doConnect(const struct sockaddr& service);

        private:
            Model model_ = Model::Unknow;
            socket_t socket_ = -1;
            std::string seesion_;
        };
    }
}

#endif // !src_network_socket_h_

