#include "src/codec/media_codec_x264.h"

#include <x264/include/x264.h>

#include "src/log.h"

namespace LJMP {
    namespace Codec {
        MediaCodecX264::MediaCodecX264(const TaskQueue::Ptr& task_queue) : MediaCodec(task_queue) {
            LOG_CREATER;
        }

        MediaCodecX264::~MediaCodecX264() {
            LOG_DESTRUCT;
        }

        CodecType MediaCodecX264::getType() {
            return CodecType::Video_X264;
        }

        bool MediaCodecX264::onInitialize(const std::shared_ptr<MediaConfig>& config) {
            return true;
        }        

        void MediaCodecX264::onunInitialize() {

        }

    }
}