#include "src/network/network_utils.h"

#include "src/utils.h"

namespace LJMP {
    namespace Network {

        bool NetworkUtils::splitHostPort(const std::string& host_port, std::string* host, short* port) {
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

        bool NetworkUtils::fillService(const char* szHost, short port, struct sockaddr_in* service) {
            if (nullptr == service) {
                return false;
            }

            service->sin_addr.s_addr = inet_addr(szHost);
            if (INADDR_NONE == service->sin_addr.s_addr) {
                struct hostent* host = gethostbyname(szHost);
                if (nullptr == host || nullptr == host->h_addr) {
                    return false;
                }
                service->sin_addr = *(struct in_addr*)host->h_addr;
            }

            service->sin_port = htons(port);
            return true;
        }

        int NetworkUtils::getSocketError() {
#ifdef WIN32
            return WSAGetLastError();
#else
            return errno;
#endif // WIN32

        }

    }
}