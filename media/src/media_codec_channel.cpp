#include "src/media_codec_channel.h"

#include "src/log.h"
#include "src/media_source_channel.h"

namespace LJMP {

    MediaCodecChannel::Ptr MediaCodecChannel::create(const TaskQueue::Ptr& task_queue) {
        return createPtr<MediaCodecChannel>(task_queue);
    }
    
    MediaCodecChannel::MediaCodecChannel(const TaskQueue::Ptr& task_queue)
        : MediaChannel(task_queue) {
        LOG_CREATER;
    }

    MediaCodecChannel::~MediaCodecChannel() {
        LOG_DESTRUCT;
    }

    void MediaCodecChannel::onUpdateScripte(const std::shared_ptr<MediaConfig>& config) {
        LOG_ENTER;

        WPtr wThis(shared_from_this());
        if (isCurrentThread()) {
            onHandleScript(config, wThis);
        }
        else {
            auto task = createTask(std::bind(&MediaCodecChannel::onHandleScript, this, config, wThis));
            invoke(task);
        }
        
    }
    
    void MediaCodecChannel::bindMediaSourceChannel(const MediaSourceChannel::Ptr& source_channel) {
        LOG_ENTER;

        if (!source_channel) {
            LOGE("source channel is nullptr");
            return;
        }
        std::shared_ptr<MediaSourceChannelDataRecive> data_recive = 
            std::dynamic_pointer_cast<MediaSourceChannelDataRecive>(shared_from_this());
        if (!data_recive) {
            LOGE("convert to media source channel data recive failed");
            return;
        }
        source_channel->registDataRecive(data_recive);
    }

    void MediaCodecChannel::onHandleScript(const std::shared_ptr<MediaConfig> config, WPtr wThis) {
        LOG_ENTER;

        TaskQueueObject::Ptr self(wThis.lock());
        if (!self) {
            LOGE("this object is nullptr");
            return;
        }
    }
}