#include "src/utils.h"

#include <string>

#ifdef WIN32
#include <io.h>

#else
#include <unistd.h>
#include <fcntl.h>

#define Access
#endif

namespace LJMP {
    bool Utils::isFileExits(const char *szPath) {
        if ( 0 == access(szPath, 0)) {
            return true;
        } else {
            return false;
        }
    }

    bool Utils::checkProtocol(const std::string& url, const char* protocol) {
        std::string sProtoc = getProtocol(url);
        return 0 == sProtoc.compare(protocol);
    }

    std::string Utils::getProtocol(const std::string& url) {
        size_t pos = url.find("://");
        if (std::string::npos == pos) {
            return "";
        }

        std::string protocol = url.substr(0, pos + 3);
        return protocol;
    }

} // namespace LJMP
