#ifndef src_media_channel_h_
#define src_media_channel_h_

#include "src/task_queue_object.h"

namespace LJMP {
    class MediaChannel : public TaskQueueObject {
        disable_copy(MediaChannel)

    public:
        ~MediaChannel() override;

    protected:
        explicit MediaChannel(const TaskQueue::Ptr& task_queue);
    };
}

#endif // !src_media_channel_h_

