#ifndef src_input_rtmp_rtmp_utils_h_
#define src_input_rtmp_rtmp_utils_h_

#include <string>

#include "src/lj_defined.h"

#include "src/input/rtmp/rtmp_define.h"

namespace LJMP {
    namespace Input {
        namespace Rtmp {
            class Utils {
                disable_copy(Utils)
            public:
                Utils() = delete;
                ~Utils() = delete;

                static bool parseUrl(const std::string& url, int* protocol, std::string* host, short* port,
                    std::string* app_name, std::string* play_path);
            };
        }
    }
}

#endif // !src_input_rtmp_utils_h_

