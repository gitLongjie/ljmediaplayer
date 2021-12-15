#ifndef src_media_codec_ffmpeg_decode_h_
#define src_media_codec_ffmpeg_decode_h_

#include "src/media_codec.h"

namespace LJMP {
    namespace Codec {
        class MediaCodecFFmpegDecode : public MediaCodec {
            disable_copy(MediaCodecFFmpegDecode)

        public:
            static CodecType getType();

        public:
            ~MediaCodecFFmpegDecode() override;

        protected:
            explicit MediaCodecFFmpegDecode(const TaskQueue::Ptr& task_queue);

        protected:
            bool onInitialize(const std::shared_ptr<MediaConfig>& config) override;
            void onUninitialize() override;
        };
    }
}

#endif // !src_media_codec_ffmpeg_decode_h_
