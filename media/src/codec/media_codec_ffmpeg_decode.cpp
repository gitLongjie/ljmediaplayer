#include "src/codec/media_codec_ffmpeg_decode.h"

#include "src/log.h"
#include "src/codec/codec_utils.h"

namespace LJMP {
    namespace Codec {

        MediaCodecFFmpegDecode::MediaCodecFFmpegDecode(const TaskQueue::Ptr& task_queue)
            : MediaCodec(task_queue) {
            LOG_CREATER;
        }

        MediaCodecFFmpegDecode::~MediaCodecFFmpegDecode() {
            LOG_DESTRUCT;
        }

        CodecType MediaCodecFFmpegDecode::getType() {
            return CodecType::Video_FFMpeg_Decode_X264;
        }

        bool MediaCodecFFmpegDecode::onInitialize(const std::shared_ptr<MediaConfig>& config) {
            LOG_ENTER;

            if (initialized) {
                LOGI("aleary is initialzed");
                return true;
            }

            if (nullptr != codec_context_) {
                LOGD("codec context is not nullptr");
                return true;
            }

            av_packet_ = av_packet_alloc();
            if (nullptr == av_packet_) {
                LOGE("alloc av packet failed");
                return false;
            }

            AVCodecID codec_id = CodecUtils::getAVCodeId(static_cast<CodecType>(config->vidoe_codec_id));
            if (AV_CODEC_ID_NONE == codec_id) {
                LOGE("codec id not find id={}", config->vidoe_codec_id);
                return false;
            }

            AVCodec* codec = avcodec_find_decoder(codec_id);
            if (nullptr == codec) {
                LOGE("not find codec");
                return false;
            }
            codec_context_ = avcodec_alloc_context3(codec);
            if (nullptr == codec_context_) {
                LOGE("avcodec alloc context3 failed");
                return false;
            }

            int ret = avcodec_open2(codec_context_, codec, nullptr);
            if (0 != ret) {
                LOGE("avcodec open failed ret={}", ret);
                return false;
            }

            AVFrame video_frame;
            initialized = true;

            return true;
        }

        void MediaCodecFFmpegDecode::onUninitialize() {
            LOG_ENTER;

            initialized = false;
            if (nullptr != codec_context_) {
                avcodec_flush_buffers(codec_context_);
                avcodec_close(codec_context_);
                avcodec_free_context(&codec_context_);
                codec_context_ = nullptr;
            }

            if (nullptr != av_packet_) {
                av_packet_free(&av_packet_);
            }
        }

    }
}