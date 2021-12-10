#ifndef src_media_source_channel_h_
#define src_media_source_channel_h_

#include <memory>

#include "src/media_channel.h"

namespace LJMP {
    class MediaSource;

    class MediaSourceChannel : public MediaChannel {
        disable_copy(MediaSourceChannel)

    public:
        using Ptr = std::shared_ptr<MediaSourceChannel>;
        static Ptr create(const std::shared_ptr<MediaSource>& media_source, const TaskQueue::Ptr& task_queue);

    public:
        ~MediaSourceChannel() override;

        void updateConfig(const MediaConfig& config);

    protected:
        MediaSourceChannel(const std::shared_ptr<MediaSource>& media_source,
            const TaskQueue::Ptr& task_queue);

    private:
        std::shared_ptr<MediaSource> media_source_;
    };
}

#endif // !src_media_source_channel_h_

