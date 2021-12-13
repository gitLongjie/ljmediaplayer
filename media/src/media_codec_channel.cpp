#include "src/media_codec_channel.h"

#include "src/log.h"
#include "src/media.h"
#include "src/media_source_channel.h"
#include "src/media_codec.h"
#include "src/media_codec_manager.h"

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

        if (!config) {
            LOGE("config is nullptr");
            return;
        }

        TaskQueueObject::Ptr self(wThis.lock());
        if (!self) {
            LOGE("this object is nullptr");
            return;
        }

        if (0 != config->vidoe_codec_id) {
            CodecType video_type = static_cast<CodecType>(config->vidoe_codec_id);
            if (vidoe_codec_) {
                vidoe_codec_->uninitialize();
                vidoe_codec_->destory();
                vidoe_codec_.reset();
            }

            vidoe_codec_ = createMediaCodec(video_type);
        }
        if (!vidoe_codec_) {
            LOGE("video codec is nullptr");
        }
        else {
            vidoe_codec_->initialize(config);
        }

        
    }

    std::shared_ptr<MediaCodec> MediaCodecChannel::createMediaCodec(CodecType type) {
        LOG_ENTER;

        if (CodecType::Unknown == type) {
            LOGE("tpye is unknown");
            return nullptr;
        }

        MediaCodecManager::Ptr media_codec_manager = Media::getInstance()->getMediaCodecManager();
        if (!media_codec_manager) {
            LOGE("media codec manager is nullptr");
            return nullptr;
        }

        MediaCodecFactory::Ptr factory = media_codec_manager->getCodecFactory(type);
        if (!factory) {
            LOGE("factory is nullptr");
            return nullptr;
        }

        const std::string name = std::string("codec_") + std::to_string(MediaCodecManager::getCodecIndex());
        TaskQueue::Ptr task_queue = TaskQueue::create(name.c_str());
        return factory->create(task_queue);
    }

}