#ifndef src_network_network_utils_h
#define src_network_network_utils_h

#include <string>

#if WIN32

#include <winsock.h>

#endif

#include "src/lj_defined.h"

namespace LJMP {
    namespace Network {
        class NetworkUtils {
            disable_copy(NetworkUtils)

        public:
            NetworkUtils() = delete;
            ~NetworkUtils() = delete;

            static bool splitHostPort(const std::string& host_port, std::string* host, short* port);
            static bool fillService(const char* szHost, short port, struct sockaddr_in* service);
            static int getSocketError();
        };
    }
}

#endif // !src_network_utils_h
