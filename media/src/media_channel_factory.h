#ifndef src_media_channel_factory_h_
#define src_media_channel_factory_h_

#include <memory>

#include "src/lj_defined.h"
#include "src/task_queue.h"

namespace LJMP {
    class MediaSource;
    class MediaSourceChannel;
    class MediaCodecChannel;

    class MediaChannelFactory {
        disable_copy(MediaChannelFactory);

    public:
        MediaChannelFactory();
        ~MediaChannelFactory();

    public:
        std::shared_ptr<MediaSourceChannel> createMediaSrouceChannel(const std::shared_ptr<MediaSource>& media_source,
            const TaskQueue::Ptr& task_queue);
        std::shared_ptr<MediaCodecChannel> createMediaCodecChannel(const TaskQueue::Ptr& task_queue);
    };
}

#endif // !src_media_channel_factory_h_

