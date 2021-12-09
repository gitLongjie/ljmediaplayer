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
        : TaskQueueObject(task_queue) {
        LOG_CREATER;
    }

    MediaContext::~MediaContext() {
        LOG_DESTRUCT;
    }

    void MediaContext::updateMediaConfig(const MediaConfig& config) {

    }

}
