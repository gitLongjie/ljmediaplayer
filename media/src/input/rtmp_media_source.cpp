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
            close();
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

        bool RTMPMediaSource::open(const std::string& url) {
            LOGI("open url {}", url);
            if (!Utils::checkProtocol(url, protocols())) {
                LOGE("protocol is not suppot");
                return false;
            }

            if (rtmp_context_) {
                rtmp_context_->uninitialzie();
            }

            rtmp_context_ = Rtmp::RtmpContext::create(url);
            if (!rtmp_context_->intialize()) {
                LOGE("rtmp conext initialize failed");
                return false;
            }
            
            return true;
        }

        void RTMPMediaSource::close() {
            LOG_ENTER;

            if (rtmp_context_) {
                rtmp_context_->uninitialzie();
                rtmp_context_.reset();
            }
        }

        const StringList& RTMPMediaSource::protocols() {
            return g_protocol;
        }

    }
}