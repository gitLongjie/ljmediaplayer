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

    bool Utils::checkProtocol(const char* url, const char* protocol) {
        std::string sUrl(url);
        size_t pos = sUrl.find("//:");
        if (std::string::npos == pos) {
            return false;
        }

        std::string sProtoc = sUrl.substr(pos);
        return 0 == sProtoc.compare(protocol);
    }

} // namespace LJMP
