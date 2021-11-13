#include "src/input/rtmp_media_source.h"

#include "src/utils.h"
#include "src/log.h"

namespace LJMP {
    namespace Input {

        static StringList g_protocol = { "rtmp://", "rtmpt://", "rtmpe://",
            "rtmpte://", "rtmps://", "rtmpts://", "rtmfp://" };

        RTMPMediaSource::RTMPMediaSource() {
            LOGI("actor {}", (long long)this);
        }

        RTMPMediaSource::~RTMPMediaSource() {
            LOGI("dactor {}", (long long)this);
        }

        bool RTMPMediaSource::checkProtocol(const std::string& protocol) {
            LOGI("check protocol: {}", protocol);

            const StringList& protos = protocols();
            auto itor = std::find(protos.begin(), protos.end(), protocol);
            if (protos.end() == itor) {
                LOGD("not support protocol: {}", protocol);
                return false;
            }
            return true;
        }

        bool RTMPMediaSource::load(const std::string& url) {
            LOGI("open url {}", url);
            if (!Utils::checkProtocol(url, protocols())) {
                LOGE("protocol is not suppot");
                return false;
            }

            return true;
        }

        const StringList& RTMPMediaSource::protocols() {
            return g_protocol;
        }

    }
}