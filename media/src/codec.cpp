#include "src/codec.h"

#include "src/log.h"

namespace LJMP {
    MediaCodec::MediaCodec(const TaskQueue::Ptr& task_queue) : TaskQueueObject(task_queue, true) {
    }

    MediaCodec::~MediaCodec() {
        LOG_DESTRUCT;
    }
    
    void MediaCodec::initialize(const std::shared_ptr<MediaConfig>& config) {
        LOG_ENTER;

        WPtr wThis(shared_from_this());
        auto task = createTask(std::bind(&MediaCodec::initializeImpl, this, config, wThis));
        invoke(task);
    }

    void MediaCodec::uninitialize() {
        WPtr wThis(shared_from_this());
        auto task = createTask(std::bind(&MediaCodec::uninitializeImpl, this, wThis));
        invoke(task);
    }

    void MediaCodec::initializeImpl(std::shared_ptr<MediaConfig> config, WPtr wThis) {
        LOG_ENTER;

        TaskQueueObject::Ptr self(wThis.lock());
        if (!self) {
            LOG_OBJECT_DESTORYED;
            return;
        }

        onInitialize(config);
    }

    void MediaCodec::uninitializeImpl(WPtr wThis) {
        LOG_ENTER;

        TaskQueueObject::Ptr self(wThis.lock());
        if (!self) {
            LOG_OBJECT_DESTORYED;
            return;
        }

        onunInitialize();
    }

}