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

            if (nullptr != codec_context_) {
                LOGD("codec context is not nullptr");
                return true;
            }

            AVCodec* codec = avcodec_find_decoder(
                CodecUtils::getAVCodeId(static_cast<CodecType>(config->vidoe_codec_id)));
            if (nullptr == codec) {
                LOGE("not find codec");
                return false;
            }
            codec_context_ = avcodec_alloc_context3(codec);
            if (nullptr == codec_context_) {
                LOGE("avcodec alloc context3 failed");
                return false;
            }
        }

        void MediaCodecFFmpegDecode::onUninitialize() {

        }

    }
}