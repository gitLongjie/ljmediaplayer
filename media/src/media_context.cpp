#include "src/media_context.h"

#include "src/log.h"

namespace LJMP {

    MediaContext::Ptr MediaContext::create(const TaskQueue::Ptr& task_queue) {
        return createPtr<MediaContext>(task_queue);
    }

    MediaContext::MediaContext(const TaskQueue::Ptr& task_queue)
        : TaskObject(task_queue, false) {
        LOG_CREATER;
    }

    MediaContext::~MediaContext() {
        LOG_DESTRUCT;
    }

    void MediaContext::updateMediaChannel(const MediaChannel::Ptr& media_channel) {
        LOG_ENTER;
        media_channel_ = media_channel;
        if (!media_channel_) {
            LOGE("media channel is nullptr");
            return;
        }
    }

    void MediaContext::updateMediaConfig(const MediaConfig& config) {
        if (isCurrentThread()) {
            onUpdateMediaConfig(config, shared_from_this());
            return;
        }

        WPtr self(shared_from_this());
        auto task = createTask(std::bind(&MediaContext::onUpdateMediaConfig, this, config, self));
        invoke(task);
    }

    void MediaContext::onUpdateMediaConfig(const MediaConfig config, WPtr wThis) {
        LOG_ENTER;

        ObjectPtr::Ptr self(wThis.lock());
        if (!self) {
            LOGE("{} is destructed", (long long)this);
            return;
        }

        media_config_ = config;
    }

}
