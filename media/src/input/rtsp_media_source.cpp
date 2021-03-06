#include "src/input/rtsp_media_source.h"

#include "src/utils.h"
#include "src/core/log.h"

namespace LJMP {
    namespace Input {

        static StringList g_protocol = { "rtsp://" };

        RTSPMediaSource::RTSPMediaSource(const std::string& url) : MediaSource(url, nullptr) {
           LOGI("actor {}", (long long)this);
        }

        RTSPMediaSource::~RTSPMediaSource() {
           LOGI("dactor {}", (long long)this);
        }

        bool RTSPMediaSource::checkProtocol(const std::string& protocol) {
            LOGI("check protocol: {}", protocol);

            const StringList& protos = protocols();
            auto itor = std::find(protos.begin(), protos.end(), protocol);
            if (protos.end() == itor) {
                LOGD("not support protocol: {}", protocol);
                return false;
            }
            return true;
        }

        const StringList& RTSPMediaSource::protocols() {
            return g_protocol;
        }

        bool RTSPMediaSource::doOpen(const std::string& url) {
            LOGI("open url {}", url);
            if (!Utils::checkProtocol(url, protocols())) {
                LOGE("protocol is not suppot");
                return false;
            }
            return true;
        }

        void RTSPMediaSource::doClose() {

        }

    }
}