#include "src/media_context.h"

#include "src/log.h"

namespace LJMP {

    MediaContext::Ptr MediaContext::create(const TaskQueue::Ptr& task_queue) {
        return createPtr<MediaContext>(task_queue);
    }

    MediaContext::MediaContext(const TaskQueue::Ptr& task_queue)
        : TaskQueueObject(task_queue, false) {
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

        media_channel_->setCallbackFunc(std::bind(&MediaContext::channelCallback, this,
            std::placeholders::_1, std::placeholders::_2));
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

    void MediaContext::channelCallback(MediaChannel::DataType type, void* data) {
        switch (type) {
        case MediaChannel::DataType::Script: {
            MediaConfig* config = reinterpret_cast<MediaConfig*>(data);
            updateMediaConfig(*config);
        }
            break;
        default:
            break;
        }
    }

    void MediaContext::onChannelCallback(MediaChannel::DataType type, void* data, WPtr wThis) {

    }

}
