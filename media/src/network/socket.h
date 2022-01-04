#ifndef src_network_socket_h_
#define src_network_socket_h_

#include <memory>

#include "src/lj_defined.h"
#include "src/core/object_sptr.h"
#include "src/kernel/read_writer.h"
#include "src/network/lj_network_define.h"

struct sockaddr;

namespace LJMP {
    namespace Network {
        class Socket : public ObjectPtr, public IReadWriter {
        public:
            enum class Model {
                Unknow,
                TCP,
                UDP
            };

            enum class ConnectStatus {
                Unknow,
                Failed,
                Connecting,
                Success
            };

            using Ptr = std::shared_ptr<Socket>;

            static Ptr create(Socket::Model model);
            static Ptr create(FD socket, Socket::Model model);
        public:
            virtual ~Socket();

            FD getFD() const override { return socket_; }
            // -1 is failed, 0 noblock 1 sucess
            virtual ConnectStatus connect(const std::string& address, short port) = 0;

            bool isTcp() const { return model_ == Model::TCP; }
            void close();

            bool enableNoDelay(bool enable);
            bool enableBlock(bool enable);
            bool enableTimeout(bool enable, int time);

            const std::string& getSessionName() const { return session_; }
            FD getSocket() const { return socket_; }

        protected:
            explicit Socket(Model model);
            Socket(FD socket, Model model);

            ConnectStatus doConnect(const struct sockaddr& service);

            void updateSession();

        private:
            Model model_ = Model::Unknow;
            FD socket_ = -1;
            std::string session_;
        };
    }
}

#endif // !src_network_socket_h_

