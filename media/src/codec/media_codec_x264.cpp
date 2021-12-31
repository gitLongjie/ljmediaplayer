#include "src/codec/media_codec_x264.h"

#include <x264/include/x264.h>

#include "src/core/log.h"

namespace LJMP {
    namespace Codec {
        const char k264CodecPreset[] = "veryfast";
        const char k264CodecTune[] = "zerolatency";

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
            LOG_ENTER;

            if (!config) {
                LOGE("config is nullptr");
                return false;
            }
            /*
            x264_param_default(&x264_param_);
            int ret = x264_param_default_preset(&x264_param_, k264CodecPreset, k264CodecTune);
            if (0 != ret) {
                LOGE("x264 param default preset failed, ret={}", ret);
                return false;
            }

            x264_param_.i_width = config->video_width;
            x264_param_.i_height = config->video_height;
            x264_param_.i_fps_den = 1;
            x264_param_.i_fps_num = config->video_frame;
            */
            return true;
        }        

        void MediaCodecX264::onUninitialize() {

        }

        void MediaCodecX264::onHandleDataBuffer(const DataBuffer::Ptr& data_buffer) {

        }

    }
}