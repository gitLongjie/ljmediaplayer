#ifndef src_input_rtmp_rtmp_utils_h_
#define src_input_rtmp_rtmp_utils_h_

#include <string>

#include "src/lj_defined.h"

#include "src/input/rtmp/rtmp_define.h"

namespace LJMP {
    namespace Input {
        namespace Rtmp {
            class RtmpUtils {
                disable_copy(RtmpUtils)
            public:
                RtmpUtils() = delete;
                ~RtmpUtils() = delete;

                static bool parseUrl(const std::string& url, int* protocol, std::string* host, short* port,
                    std::string* app_name, std::string* play_path);

                static char* amfEncodeString(char* output, char* outend, const std::string& str);
                static char* amfEncodeNumber(char* output, char* outend, double dVal);
                static char* amfEncodeInt16(char* output, char* outend, short nVal);
                static char* amfEncodeInt24(char* output, char* outend, int nVal);
                static char* amfEncodeInt32(char* output, char* outend, int nVal);
                static char* amfEncodeBoolean(char* output, char* outend, int bVal);

                static char* amfEncodeNamedString(char* output, char* outend, const std::string& name, const std::string& value);
                static char* amfEncodeNamedNumber(char* output, char* outend, const std::string& name, double dVal);
                static char* amfEncodeNamedBoolean(char* output, char* outend, const std::string& name, int bVal);
            };
        }
    }
}

#endif // !src_input_rtmp_utils_h_

