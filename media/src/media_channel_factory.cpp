#include "src/media_channel_factory.h"

#include "src/core/log.h"

#include "src/media_source.h"
#include "src/media_source_channel.h"
#include "src/media_codec_channel.h"

namespace LJMP {
    MediaChannelFactory::MediaChannelFactory() {
        LOG_CREATER;
    }

    MediaChannelFactory::~MediaChannelFactory() {
        LOG_DESTRUCT;
    }
    std::shared_ptr<MediaSourceChannel> MediaChannelFactory::createMediaSrouceChannel(
        const std::shared_ptr<MediaSource>& media_source, const TaskQueue::Ptr& task_queue) {
        return MediaSourceChannel::create(media_source, task_queue);
    }
    std::shared_ptr<MediaCodecChannel> MediaChannelFactory::createMediaCodecChannel(const TaskQueue::Ptr& task_queue) {
        return MediaCodecChannel::create(task_queue);
    }
}