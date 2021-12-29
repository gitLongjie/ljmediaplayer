#ifndef src_codec_codec_manager
#define src_codec_codec_manager

#include "src/task_object.h"

#include "src/media_codec_factory.h"

namespace LJMP {

    class MediaCodecManager : public TaskObject {
        disable_copy(MediaCodecManager)

    public:
        using Ptr = std::shared_ptr<MediaCodecManager>;
        static Ptr create(const TaskQueue::Ptr& task_queue);

    public:
        ~MediaCodecManager() override = default;

        virtual bool initialize() = 0;
        virtual void uninitialize() = 0;
        virtual MediaCodecFactory::Ptr getCodecFactory(CodecType type) const = 0;

        static long long getCodecIndex();

    protected:
        explicit MediaCodecManager(const TaskQueue::Ptr& task_queue) : TaskObject(task_queue, false){}
    };
}
#endif // !src_codec_codec_manager

