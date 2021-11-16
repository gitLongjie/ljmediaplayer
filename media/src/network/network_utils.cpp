#include "src/network/network_utils.h"

#include "src/utils.h"

namespace LJMP {
    namespace Network {

        bool Utils::splitHostPort(const std::string& host_port, std::string* host, short* port) {
            if (nullptr == host || nullptr == port) {
                return false;
            }

            std::vector<std::string> sp = LJMP::Utils::split(host_port, ":");
            if (sp.size() > 2) {
                return false;
            }

            *host = sp[0];
            if (sp.size() == 2) {
                int t = atoi(sp[1].c_str());
                if (t > 65535) {
                    *port = 0;
                }
                else {
                    *port = t;
                }
            }
            return true;
        }

    }
}