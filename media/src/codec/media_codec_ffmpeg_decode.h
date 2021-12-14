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
        };
    }
}

#endif // !src_media_codec_ffmpeg_decode_h_
