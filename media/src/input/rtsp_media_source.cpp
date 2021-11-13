#include "src/input/rtsp_media_source.h"

#include "src/utils.h"
#include "src/log.h"

namespace LJMP {
    namespace Input {

        RTSPMediaSource::RTSPMediaSource() {
           LOGI("actor {}", (long long)this);
        }

        RTSPMediaSource::~RTSPMediaSource() {
           LOGI("dactor {}", (long long)this);
        }

        bool RTSPMediaSource::load(const std::string& url) {
            LOGI("open url {}", url);
            if (!Utils::checkProtocol(url, protocol())) {
                LOGE("protocol is not suppot");
                return false;
            }

            return true;
        }

        const char* RTSPMediaSource::protocol() {
            return "rtsp://";
        }

    }
}