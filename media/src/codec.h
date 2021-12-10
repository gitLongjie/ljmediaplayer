#ifndef src_codec_h_
#define src_codec_h_

#include "src/task_queue_object.h"

namespace LJMP {
    class MediaCodec : public TaskQueueObject {
        disable_copy(MediaCodec)

    public:
        using Ptr = std::shared_ptr<MediaCodec>;
        static Ptr create();

    public:
        virtual t
    };
}

#endif // !src_codec_h_

