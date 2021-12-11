#include "src/input/rtmp_media_source.h"

#include "src/utils.h"
#include "src/log.h"
#include "src/media.h"
#include "src/media_source_channel.h"
#include "ljmedia/error_code.h"

#include "src/input/rtmp/rtmp_utils.h"

static const char kAudioDatarate[] = "audiodatarate";
static const char kAudioChannel[] = "audiochannel";
static const char kAudioCodecId[] = "audiocodecid";

static const char kVideoFramerate[] = "framerate";
static const char kVideoWidth[] = "width";
static const char kVideoHeight[] = "height";
static const char kVideoCodecId[] = "videocodecid";

namespace LJMP {
    namespace Input {

        static StringList g_protocol = { "rtmp://", "rtmpt://", "rtmpe://",
            "rtmpte://", "rtmps://", "rtmpts://", "rtmfp://" };

        RTMPMediaSource::RTMPMediaSource(const std::string& url)
            : MediaSource(url, Media::getInstance()->getIOTaskQueue()) {
            LOG_CREATER;
        }

        RTMPMediaSource::~RTMPMediaSource() {
            LOG_DESTRUCT;
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

            RTMPMediaSource::Ptr self = std::dynamic_pointer_cast<RTMPMediaSource>(shared_from_this());
            rtmp_context_ = Rtmp::RtmpContext::create(getTaskQueue(), self, url);
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

            TaskQueueObject::Ptr self(wThis.lock());
            if (!self) {
                LOGE("this object destructed");
                return;
            }

            doClose();
        }

        void RTMPMediaSource::onHandleScripte(const DataBuffer::Ptr& data_buffer) {
            const char* data = data_buffer->getData();
            const unsigned int data_length = data_buffer->getSize();
            int audio_rate = static_cast<int>(Rtmp::RtmpUtils::getAFMMetaDataByName(
                data, data_length, kAudioDatarate, sizeof(kAudioDatarate)));
            int audio_channel = static_cast<int>(Rtmp::RtmpUtils::getAFMMetaDataByName(
                data, data_length, kAudioChannel, sizeof(kAudioChannel)));
            int audio_code_id = static_cast<int>(Rtmp::RtmpUtils::getAFMMetaDataByName(
                data, data_length, kAudioCodecId, sizeof(kAudioCodecId)));
            LOGI("audio rate={} audio_channel={} audio code id= {}", audio_rate, audio_channel, audio_code_id);

            int video_frame = static_cast<int>(Rtmp::RtmpUtils::getAFMMetaDataByName(
                data, data_length, kVideoFramerate, sizeof(kVideoFramerate)));
            int video_width = static_cast<int>(Rtmp::RtmpUtils::getAFMMetaDataByName(
                data, data_length, kVideoWidth, sizeof(kVideoWidth)));
            int video_height = static_cast<int>(Rtmp::RtmpUtils::getAFMMetaDataByName(
                data, data_length, kVideoHeight, sizeof(kVideoHeight)));
            int video_code_id = static_cast<int>(Rtmp::RtmpUtils::getAFMMetaDataByName(
                data, data_length, kVideoCodecId, sizeof(kVideoCodecId)));
            LOGI("video frame={} video width={} video height={} video code id= {}", video_frame, video_width,
                video_height, video_code_id);

            callbackFunc callback = getCallbackFunc();
            if (!callback) {
                LOGE("callback is nullptr");
                return;
            }
            audio_code_id = audio_code_id ? audio_code_id : static_cast<int>(CodecType::Audio_AAC);
            video_code_id = video_code_id ? video_code_id : static_cast<int>(CodecType::Video_X264);
            if (video_code_id == 7) {
                video_code_id = static_cast<int>(CodecType::Video_X264);
            }

            MediaConfig config = { audio_rate, audio_channel, audio_code_id,
                video_width, video_height, video_frame, video_code_id};
            callback(DataType::Script, &config);
        }



    }
}