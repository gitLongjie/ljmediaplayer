#include "src/media_codec_channel.h"

#include "src/log.h"
#include "src/media.h"
#include "src/media_source_channel.h"
#include "src/media_codec.h"
#include "src/media_codec_manager.h"
#include "src/data_buffer.h"

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

    void MediaCodecChannel::onReciveScripte(const std::shared_ptr<MediaConfig>& config) {
        LOG_ENTER;

        WPtr wThis(shared_from_this());
        auto task = createTask(std::bind(&MediaCodecChannel::onHandleScript, this, config, wThis));
        invoke(task);
    }
    
    void MediaCodecChannel::onReciveVideo(const DataBuffer::Ptr& data_buffer) {
        WPtr wThis(shared_from_this());
        auto task = createTask(std::bind(&MediaCodecChannel::onHandleVideo, this, data_buffer, wThis));
        invoke(task);
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

        TaskObject::Ptr self(wThis.lock());
        if (!self) {
            LOGE("this object is nullptr");
            return;
        }

        if (0 != config->vidoe_codec_id) {
            CodecType video_type = static_cast<CodecType>(config->vidoe_codec_id);
            if (video_codec_) {
                video_codec_->uninitialize();
                video_codec_->destory();
                video_codec_.reset();
            }

            video_codec_ = createMediaCodec(video_type);
        }
        if (!video_codec_) {
            LOGE("video codec is nullptr");
        }
        else {
            video_codec_->initialize(config);
        }

        
    }

    void MediaCodecChannel::onHandleVideo(const DataBuffer::Ptr data_buffer, WPtr wThis) {
        TaskObject::Ptr self(wThis.lock());
        if (!self) {
            LOG_DESTRUCT;
            return;
        }

        if (nullptr == video_codec_) {
            LOGE("video cdec is nullptr");
            return;
        }

        video_codec_->pushVideoDataBuffer(data_buffer);

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