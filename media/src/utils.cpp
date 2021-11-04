#include "src/utils.h"

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

} // namespace LJMP
