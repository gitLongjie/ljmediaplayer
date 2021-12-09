#ifndef src_media_context_h_
#define src_media_context_h_

#include <memory>

#include "src/lj_defined.h"
#include "src/task_queue_object.h"

namespace LJMP {
    class MediaSource;

    struct MediaConfig {
        int audio_sample;
        int audio_channel;
        int audio_codec_id;

        int video_width;
        int video_height;
        int video_frame;
        int vidoe_codec_id;
    };

    class MediaContext : public TaskQueueObject {
        disable_copy(MediaContext)

    public:
        using Ptr = std::shared_ptr<MediaContext>;

        static Ptr create(const TaskQueue::Ptr& task_queue);

    public:
        ~MediaContext() override;

        void updateMediaSrouce(const std::shared_ptr<MediaSource>& media_source) {
            media_source_ = media_source;
        }

        void updateMediaConfig(const MediaConfig& config);

    protected:
        explicit MediaContext(const TaskQueue::Ptr& task_queue);

    private:
        std::shared_ptr<MediaSource> media_source_;
    };
}

#endif // !src_media_context_h_

