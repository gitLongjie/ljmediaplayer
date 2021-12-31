#ifndef src_codec_h_
#define src_codec_h_

#include <queue>

#include "src/core/task_object.h"
#include "src/core/data_buffer.h"

namespace LJMP {
    class MediaCodec : public TaskObject {
        disable_copy(MediaCodec)

    public:
        using Ptr = std::shared_ptr<MediaCodec>;

    public:
        ~MediaCodec() override;

        void initialize(const std::shared_ptr<MediaConfig>& config);
        void uninitialize();

        void pushVideoDataBuffer(const DataBuffer::Ptr& data_buffer);

    protected:
        explicit MediaCodec(const TaskQueue::Ptr& task_queue);

        virtual bool onInitialize(const std::shared_ptr<MediaConfig>& config) = 0;
        virtual void onUninitialize() = 0;
        virtual void onHandleDataBuffer(const DataBuffer::Ptr& data_buffer) = 0;

        DataBuffer::Ptr pullDataBuffer();

    private:
        void initializeImpl(std::shared_ptr<MediaConfig> config, WPtr wThis);
        void uninitializeImpl(WPtr wThis);
        void handleDataBuffer(WPtr wThis);

    private:
        unsigned int max_cache_count = 10;
        std::mutex mutex_;
        DataBuffer::Queue data_buffer_queue_;
    };
}

#endif // !src_codec_h_

