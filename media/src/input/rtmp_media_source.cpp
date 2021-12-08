#include "src/input/rtmp_media_source.h"

#include "src/utils.h"
#include "src/log.h"
#include "src/media.h"
#include "ljmedia/error_code.h"

namespace LJMP {
    namespace Input {

        static StringList g_protocol = { "rtmp://", "rtmpt://", "rtmpe://",
            "rtmpte://", "rtmps://", "rtmpts://", "rtmfp://" };

        RTMPMediaSource::RTMPMediaSource() : MediaSource(Media::getInstance()->getIOTaskQueue()) {
            LOGI("actor {}", (long long)this);
        }

        RTMPMediaSource::~RTMPMediaSource() {
            LOGI("dactor {}", (long long)this);
        }

        void RTMPMediaSource::errorCallback(int code, const char* msg) {
            WPtr wThis(shared_from_this());
            std::string message(msg);
            auto task = createTask(std::bind(&RTMPMediaSource::doErrorCallback, this, code, message, wThis));
            invoke(task);
            if (error_code_netork_connect_failed == code) {
                Media::getInstance()->errorCallbak(error_code_netork_connect_failed, "connect server failed");
            }
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

        const StringList& RTMPMediaSource::protocols() {
            return g_protocol;
        }


        bool RTMPMediaSource::doOpen(const std::string& url) {
            if (!Utils::checkProtocol(url, protocols())) {
                LOGE("protocol is not suppot");
                return false;
            }

            if (rtmp_context_) {
                rtmp_context_->uninitialzie();
            }

            rtmp_context_ = Rtmp::RtmpContext::create(shared_from_this(), url);
            if (!rtmp_context_->intialize()) {
                LOGE("rtmp conext initialize failed");
                return false;
            }

            return true;
        }

        void RTMPMediaSource::doClose() {
            LOG_ENTER;

            if (rtmp_context_) {
                rtmp_context_->uninitialzie();
                rtmp_context_.reset();
            }
        }

        void RTMPMediaSource::doErrorCallback(int code, std::string msg, WPtr wThis) {
            LOGI("code={} message={}", code, msg);

            Media::getInstance()->errorCallbak(code, msg.c_str());

            Ptr self(wThis.lock());
            if (!self) {
                LOGE("this object destructed");
                return;
            }

            doClose();

        }

    }
}