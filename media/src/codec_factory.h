#ifndef src_codec_factory_h_
#define  src_codec_factory_h_

#include "src/lj_defined.h"

namespace LJMP {
    class Codec;

    class CodecFactory {
        disable_copy(CodecFactory)

    public:
        virtual ~CodecFactory() = default;
        virtual CodecType getCodeType() const = 0;
        virtual std::shared_ptr<Codec> create() = 0;
    };
}

#endif // !src_codec_factory_h_

