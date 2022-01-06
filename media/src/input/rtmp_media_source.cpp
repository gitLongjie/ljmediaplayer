#include "src/input/rtmp_media_source.h"

#include "src/utils.h"
#include "src/core/log.h"
#include "src/core/enum_operator.h"
#include "src/kernel/channel.h"
#include "src/kernel/network_manager.h"
#include "src/media.h"
#include "src/media_source_channel.h"
#include "ljmedia/error_code.h"

#include "src/network/network_utils.h"
#include "src/network/socket_factory.h"
#include "src/kernel/channel_factory.h"

#include "src/input/rtmp/rtmp_read_writer_callback.h"

#include "src/input/rtmp/rtmp_utils.h"
#include "src/input/rtmp/rtmp_status.h"

static const char kAudioDatarate[] = "audiodatarate";
static const char kAudioChannel[] = "audiochannel";
static const char kAudioCodecId[] = "audiocodecid";

static const char kVideoFramerate[] = "framerate";
static const char kVideoWidth[] = "width";
static const char kVideoHeight[] = "height";
static const char kVideoCodecId[] = "videocodecid";

short kDefaultPort = 1935;

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
            INetworkManager::Ptr network_manager = Media::getInstance()->getNetworkManager();
            if (!network_manager) {
                LOGE("network manger is nullptr");
                return false;
            }

            if (!Utils::checkProtocol(url, protocols())) {
                LOGE("protocol is not suppot");
                return false;
            }

            if (rtmp_context_) {
                rtmp_context_->uninitialzie();
            }

            std::string host;
            short port = kDefaultPort;
            if (!Network::NetworkUtils::splitHostPort(url, &host, &port)) {
                LOGE("splict host port failed");
                return false;
            }
            if (port == 0) {
                port = kDefaultPort;
            }

            Network::SocketFactory factory;
            Network::Socket::Ptr socket = factory.createTcp();
            socket->enableBlock(false);

            std::weak_ptr<RTMPMediaSource> wThis(std::dynamic_pointer_cast<RTMPMediaSource>(shared_from_this()));
            rtmp_context_ = Rtmp::RtmpContext::create(getTaskQueue(), wThis, url);
            if (!rtmp_context_->intialize()) {
                LOGE("rtmp conext initialize failed");
                return false;
            }

            std::shared_ptr<Rtmp::RtmpReadWriteCallback> callback = createPtr<Rtmp::RtmpReadWriteCallback>(rtmp_context_);

            ChannelFactory channel_factory;
            channel_ = channel_factory.create(socket, callback);
            
            if (Network::Socket::ConnectStatus::Connecting == socket->connect(host, port)) {
                rtmp_context_->switchStatus(createPtr<Rtmp::RtmpConnectStatus>(rtmp_context_));
                Media::getInstance()->getNetworkManager()->addConnectChannel(channel_);
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

            ObjectPtr::Ptr self(wThis.lock());
            if (!self) {
                LOGE("this object destructed");
                return;
            }

            doClose();
        }
/*
        void RTMPMediaSource::onHandleScripte(const DataBuffer::Ptr& data_buffer) {
            const char* data = data_buffer->getData();
            const unsigned int data_length = data_buffer->getSize();
            int audio_rate = static_cast<int>(Rtmp::RtmpUtils::getAFMMetaDataByName(
                data, data_length, kAudioDatarate, sizeof(kAudioDatarate)));
            int audio_channel = static_cast<int>(Rtmp::RtmpUtils::getAFMMetaDataByName(
                data, data_length, kAudioChannel, sizeof(kAudioChannel)));
            int audio_codec_id = static_cast<int>(Rtmp::RtmpUtils::getAFMMetaDataByName(
                data, data_length, kAudioCodecId, sizeof(kAudioCodecId)));
            LOGI("audio rate={} audio_channel={} audio code id= {}", audio_rate, audio_channel, audio_codec_id);

            int video_frame = static_cast<int>(Rtmp::RtmpUtils::getAFMMetaDataByName(
                data, data_length, kVideoFramerate, sizeof(kVideoFramerate)));
            int video_width = static_cast<int>(Rtmp::RtmpUtils::getAFMMetaDataByName(
                data, data_length, kVideoWidth, sizeof(kVideoWidth)));
            int video_height = static_cast<int>(Rtmp::RtmpUtils::getAFMMetaDataByName(
                data, data_length, kVideoHeight, sizeof(kVideoHeight)));
            int video_codec_id = static_cast<int>(Rtmp::RtmpUtils::getAFMMetaDataByName(
                data, data_length, kVideoCodecId, sizeof(kVideoCodecId)));
            LOGI("video frame={} video width={} video height={} video code id= {}", video_frame, video_width,
                video_height, video_codec_id);

            callbackFunc callback = getCallbackFunc();
            if (!callback) {
                LOGE("callback is nullptr");
                return;
            }

            video_codec_id_ = video_codec_id;
            audio_codec_id = audio_codec_id ? audio_codec_id : static_cast<int>(CodecType::Audio_AAC);
            video_codec_id = static_cast<int>(CodecType::Video_FFMpeg_Decode);

            MediaConfig config = { audio_rate, audio_channel, audio_codec_id,
                video_width, video_height, video_frame, video_codec_id };
            callback(DataType::Script, &config);
        }

        void RTMPMediaSource::OnHandleVideoData(const DataBuffer::Ptr& data_buffer) {
            callbackFunc callback = getCallbackFunc();
            if (!callback) {
                LOGE("callback is nullptr");
                return;
            }
           
            const char* data = data_buffer->getData();
            const FLVVideoTagHeader* video_tage_header = reinterpret_cast<const FLVVideoTagHeader*>(data);
            if (video_codec_id_ != video_tage_header->codec_id) {
                return;
            }
            int frame_type = video_tage_header->frame_type;
            int len = sizeof(FLVVideoTagHeader);
            data_buffer->setOffset(sizeof(FLVVideoTagHeader));
            callback(DataType::Video, static_cast<const void*>(&data_buffer));
        }
        */
    }
}