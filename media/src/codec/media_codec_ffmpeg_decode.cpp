#include "src/codec/media_codec_ffmpeg_decode.h"

#include "src/log.h"

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
            return CodecType::Video_FFMpeg_Decode;
        }

        bool MediaCodecFFmpegDecode::onInitialize(const std::shared_ptr<MediaConfig>& config) {

        }

        void MediaCodecFFmpegDecode::onunInitialize() {

        }

    }
}