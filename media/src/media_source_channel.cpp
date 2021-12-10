#include "src/media_source_channel.h"

#include "src/log.h"
#include "src/media_source.h"

namespace LJMP {
    MediaSourceChannel::Ptr MediaSourceChannel::create(const MediaSource::Ptr& media_source, const TaskQueue::Ptr& task_queue) {
        return createPtr<MediaSourceChannel>(media_source, task_queue);
    }

    MediaSourceChannel::MediaSourceChannel(const MediaSource::Ptr& media_source, const TaskQueue::Ptr& task_queue)
        : MediaChannel(task_queue)
        , media_source_(media_source) {

    }

    MediaSourceChannel::~MediaSourceChannel() {

    }

    void MediaSourceChannel::updateConfig(const MediaConfig& config) {
        if (callback_func_) {
            callback_func_(DataType::Script, static_cast<void*>(const_cast<MediaConfig*>(&config)));
        }
    }

}