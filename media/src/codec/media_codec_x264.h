#ifndef src_codec_media_codec_x264_h_
#define src_codec_media_codec_x264_h_

#include "src/lj_defined.h"
#include "src/codec.h"

namespace LJMP {
    namespace Codec {
        class MediaCodecX264 : public MediaCodec {
            disable_copy(MediaCodecX264)

        public:
            static CodecType getType();

        public:
            ~MediaCodecX264() override;

        protected:
            explicit MediaCodecX264(const TaskQueue::Ptr& task_queue);

        protected:
            bool onInitialize(const std::shared_ptr<MediaConfig>& config) override;
            void onunInitialize() override;
        };
    }
}

#endif // !src_codec_media_codec_x264_h_

