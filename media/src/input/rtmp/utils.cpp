#include "src/input/rtmp/utils.h"

namespace LJMP {
    namespace Input {
        namespace Rtmp {

            bool Utils::parseUrl(const char* url, int* protocol, AVal* host, unsigned int* port, AVal* playpath,
                AVal* app) {
                *protocol = RTMP_PROTOCOL_RTMP;
                *port = 0;
                playpath->av_len = 0;
                playpath->av_val = nullptr;
                app->av_len = 0;
                app->av_val = nullptr;
            }

        }
    }
}