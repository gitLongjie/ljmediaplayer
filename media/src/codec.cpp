#include "src/codec.h"

#include "src/log.h"

namespace LJMP {
    MediaCodec::~MediaCodec() {
        LOG_DESTRUCT;
    }
}