#ifndef src_codec_h_
#define src_codec_h_

#include "src/task_queue_object.h"

namespace LJMP {
    class MediaCodec : public TaskQueueObject {
        disable_copy(MediaCodec)

    public:
        using Ptr = std::shared_ptr<MediaCodec>;

    public:
        ~MediaCodec() override;

        void initialize(const std::shared_ptr<MediaConfig>& config);
        void uninitialize();

    protected:
        explicit MediaCodec(const TaskQueue::Ptr& task_queue);

        virtual bool onInitialize(const std::shared_ptr<MediaConfig>& config) = 0;
        virtual void onUninitialize() = 0;

    private:
        void initializeImpl(std::shared_ptr<MediaConfig> config, WPtr wThis);
        void uninitializeImpl(WPtr wThis);
    };
}

#endif // !src_codec_h_

