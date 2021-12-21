#include "src/codec/codec_utils.h"

namespace LJMP {
    namespace Codec {
        
        AVCodecID CodecUtils::getAVCodeId(CodecType codec) {
            switch (codec)
            {
            case CodecType::Unknown:
                return AV_CODEC_ID_NONE;
            case CodecType::Audio_AAC:
                break;
            case CodecType::Video_X264:
            case CodecType::Video_FFMpeg_Decode:
                return AV_CODEC_ID_H264;
            default:
                break;
            }
            return AV_CODEC_ID_NONE;
        }

    }
}