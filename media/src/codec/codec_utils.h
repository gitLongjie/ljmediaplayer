#ifndef src_ceodec_codec_utils_h_
#define src_ceodec_codec_utils_h_

#include "src/lj_defined.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>

#ifdef __cplusplus
}
#endif

namespace LJMP {
    namespace Codec {
        class CodecUtils {
            disable_copy(CodecUtils)

        public:
            CodecUtils() = delete;
            ~CodecUtils() = delete;

            static AVCodecID getAVCodeId(CodecType codec);
        };
    }
}
#endif // !src_ceodec_codec_utils_h_

