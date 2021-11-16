#ifndef src_network_network_utils_h
#define src_network_network_utils_h

#include <string>

#include "src/lj_defined.h"

namespace LJMP {
    namespace Network {
        class Utils {
            disable_copy(Utils)

        public:
            Utils() = delete;
            ~Utils() = delete;

            static bool splitHostPort(const std::string& host_port, std::string* host, short* port);
        };
    }
}

#endif // !src_network_utils_h
