#include "src/media_source_channel.h"

#include "src/log.h"
#include "src/media_source.h"
#include "src/media_source_channel_data_recive.h"

namespace LJMP {
    MediaSourceChannel::Ptr MediaSourceChannel::create(const MediaSource::Ptr& media_source,
        const TaskQueue::Ptr& task_queue) {
        Ptr self = createPtr<MediaSourceChannel>(media_source, task_queue);
        WPtr wThis(self);
        media_source->setCallbackFunc(std::bind(&MediaSourceChannel::reciveData, self.get(),
            std::placeholders::_1, std::placeholders::_2, wThis));
        return self;
    }

    MediaSourceChannel::MediaSourceChannel(const MediaSource::Ptr& media_source, const TaskQueue::Ptr& task_queue)
        : MediaChannel(task_queue)
        , media_source_(media_source) {
        LOG_CREATER;
    }

    MediaSourceChannel::~MediaSourceChannel() {
        LOG_DESTRUCT;
    }

    void MediaSourceChannel::registDataRecive(const std::shared_ptr<MediaSourceChannelDataRecive>& data_recive) {
        LOG_ENTER;
        WPtr wThis(shared_from_this());
        if (isCurrentThread()) {
            onRegistDataRecive(data_recive, wThis);
        }
        else {
            auto task = createTask(std::bind(&MediaSourceChannel::onRegistDataRecive, this, data_recive, wThis));
            invoke(task);
        }
    }

    void MediaSourceChannel::unregistDataRecive(const std::shared_ptr<MediaSourceChannelDataRecive>& data_recive) {
        LOG_ENTER;

        WPtr wThis(shared_from_this());
        if (isCurrentThread()) {
            onUnregistDataRecive(data_recive, wThis);
        }
        else {
            auto task = createTask(std::bind(&MediaSourceChannel::onUnregistDataRecive, this, data_recive, wThis));
            invoke(task);
        }
    }

    bool MediaSourceChannel::start() {
        LOG_ENTER;

        WPtr wThis(shared_from_this());
        if (isCurrentThread()) {
            onStart(wThis);
        }
        else {
            auto task = createTask(std::bind(&MediaSourceChannel::onStart, this, wThis));
            invoke(task);
        }
        return true;
    }

    void MediaSourceChannel::reciveData(DataType data_type, void* data, WPtr wThis) {
        TaskQueueObject::Ptr self(wThis.lock());
        if (!self) {
            LOGE("this object is destructed");
            return;
        }

        if (nullptr == data) {
            LOGE("data is nullptr");
            return;
        }

        switch (data_type) {
        case DataType::Script: {
            MediaConfig* config = reinterpret_cast<MediaConfig*>(data);
            std::shared_ptr<MediaConfig> media_config = std::make_shared<MediaConfig>(*config);
            if (isCurrentThread()) {
                onUpdateScript(media_config, wThis);
            }
            else {
                auto task = createTask(std::bind(&MediaSourceChannel::onUpdateScript, this, media_config, wThis));
                invoke(task);
            }
        } break;
        default:
            break;
        }

    }

    void MediaSourceChannel::onRegistDataRecive(const std::shared_ptr<MediaSourceChannelDataRecive> data_recive, WPtr wThis) {
        LOG_ENTER;
        TaskQueueObject::Ptr self(wThis.lock());
        if (!self) {
            LOGE("this object is destruct");
            return;
        }

        const auto itor = std::find(list_data_recives_.begin(), list_data_recives_.end(), data_recive);
        if (list_data_recives_.end() != itor) {
            LOGD("data recive already exist");
            return;
        }
        list_data_recives_.push_back(data_recive);
    }

    void MediaSourceChannel::onUnregistDataRecive(const std::shared_ptr<MediaSourceChannelDataRecive> data_recive, WPtr wThis) {
        LOG_ENTER;

        TaskQueueObject::Ptr self(wThis.lock());
        if (!self) {
            LOGE("this object is destruct");
            return;
        }

        const auto itor = std::find(list_data_recives_.begin(), list_data_recives_.end(), data_recive);
        if (list_data_recives_.end() == itor) {
            LOGD("data recive does not exist");
            return;
        }
        list_data_recives_.erase(itor);
    }

    void MediaSourceChannel::onUpdateScript(const std::shared_ptr<MediaConfig>& config, WPtr wThis) {
        LOG_ENTER;

        TaskQueueObject::Ptr self(wThis.lock());
        if (!self) {
            LOGE("this object is desturct");
            return;
        }

        for (auto& item : list_data_recives_) {
            item->onUpdateScripte(config);
        }
    }

    void MediaSourceChannel::onStart(WPtr wThis) {
        LOG_ENTER;

        TaskQueueObject::Ptr self(wThis.lock());
        if (!self) {
            LOGE("this object is destruct");
            return;
        }

        if (!media_source_) {
            LOGE("media source is nullptr");
            return;
        }

        media_source_->start();
    }

}