#include "src/codec.h"

#include "src/log.h"

namespace LJMP {
    MediaCodec::MediaCodec(const TaskQueue::Ptr& task_queue) : TaskQueueObject(task_queue, true) {
    }

    MediaCodec::~MediaCodec() {
        LOG_DESTRUCT;
    }
    
}