#include "src/media_channel.h"

#include "src/log.h"

namespace LJMP {
    MediaChannel::MediaChannel(const TaskQueue::Ptr& task_queue) : TaskObject(task_queue, false) {
        LOG_CREATER;
    }

    MediaChannel::~MediaChannel() {
        LOG_DESTRUCT;
    }
}
