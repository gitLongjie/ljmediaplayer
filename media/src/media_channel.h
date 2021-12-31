#ifndef src_media_channel_h_
#define src_media_channel_h_

#include "src/core/task_object.h"

namespace LJMP {
    class MediaChannel : public TaskObject {
        disable_copy(MediaChannel)

    public:
        using Ptr = std::shared_ptr<MediaChannel>;
        using callbackFunc = std::function<void(DataType type, void* data)>;

    public:
        ~MediaChannel() override;

        void setCallbackFunc(callbackFunc func) {
            callback_func_ = func;
        }

    protected:
        explicit MediaChannel(const TaskQueue::Ptr& task_queue);

    protected:
        callbackFunc callback_func_;
    };
}

#endif // !src_media_channel_h_

