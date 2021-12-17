#include "src/media_codec.h"

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

    void MediaCodec::pushVideoDataBuffer(const DataBuffer::Ptr& data_buffer) { 
        {
            std::lock_guard<std::mutex> lock(mutex_);
            while (data_buffer_queue_.size() > max_cache_count) {
                data_buffer_queue_.pop();
            }
            data_buffer_queue_.push(data_buffer);
        }

        WPtr wThis(shared_from_this());
        auto task = createTask(std::bind(&MediaCodec::handleDataBuffer, this, wThis));
        invoke(task);
    }

    DataBuffer::Ptr MediaCodec::pullDataBuffer() {
        DataBuffer::Ptr ret;
        {
            std::lock_guard<std::mutex> lock(mutex_);
            if (data_buffer_queue_.empty()) {
                return ret;
            }

            ret = data_buffer_queue_.front();
            data_buffer_queue_.pop();
        }
        return ret;
    }



    void MediaCodec::initializeImpl(std::shared_ptr<MediaConfig> config, WPtr wThis) {
        LOG_ENTER;

        TaskQueueObject::Ptr self(wThis.lock());
        if (!self) {
            LOG_OBJECT_DESTORYED;
            return;
        }

        if (!onInitialize(config)) {
            LOGE("initialize failed");
            onUninitialize();
        }
    }

    void MediaCodec::uninitializeImpl(WPtr wThis) {
        LOG_ENTER;

        TaskQueueObject::Ptr self(wThis.lock());
        if (!self) {
            LOG_OBJECT_DESTORYED;
            return;
        }

        onUninitialize();
    }

    void MediaCodec::handleDataBuffer(WPtr wThis) {
        TaskQueueObject::Ptr self(wThis.lock());
        if (!self) {
            LOG_OBJECT_DESTORYED;
            return;
        }
        DataBuffer::Ptr data_bffer = pullDataBuffer();
        if (!data_bffer) {
            LOGE("data is empty");
            return;
        }
        onHandleDataBuffer(data_bffer);
    }

}