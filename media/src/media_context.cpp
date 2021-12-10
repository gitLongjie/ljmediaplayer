#include "src/media_context.h"

#include "src/log.h"

namespace LJMP {

    MediaContext::Ptr MediaContext::create(const TaskQueue::Ptr& task_queue) {
        struct Creator : public MediaContext {
            Creator(const TaskQueue::Ptr& task_queue) : MediaContext(task_queue) {}
            ~Creator() override = default;
        };

        return std::make_shared<Creator>(task_queue);
    }

    MediaContext::MediaContext(const TaskQueue::Ptr& task_queue)
        : TaskQueueObject(task_queue, false) {
        LOG_CREATER;
    }

    MediaContext::~MediaContext() {
        LOG_DESTRUCT;
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

        TaskQueueObject::Ptr self(wThis.lock());
        if (!self) {
            LOGE("{} is destructed", (long long)this);
            return;
        }

        media_config_ = config;
    }

}
