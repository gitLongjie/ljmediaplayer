#ifndef src_media_context_h_
#define src_media_context_h_

#include <memory>

#include "src/lj_defined.h"
#include "src/task_queue_object.h"
#include "src/media_channel.h"

namespace LJMP {
    class MediaChannel;
    class MediaCodec;

    class MediaContext : public TaskQueueObject {
        disable_copy(MediaContext)

    public:
        using Ptr = std::shared_ptr<MediaContext>;

        static Ptr create(const TaskQueue::Ptr& task_queue);

    public:
        ~MediaContext() override;

        void updateMediaChannel(const MediaChannel::Ptr& media_channel);

        void updateMediaConfig(const MediaConfig& config);

    protected:
        explicit MediaContext(const TaskQueue::Ptr& task_queue);

        void onUpdateMediaConfig(const MediaConfig config, WPtr wThis);

    private:
        MediaChannel::Ptr media_channel_;
        std::shared_ptr<MediaCodec> media_codec_;

        MediaConfig media_config_;
    };
}

#endif // !src_media_context_h_

