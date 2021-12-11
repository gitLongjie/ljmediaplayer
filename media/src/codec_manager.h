#ifndef src_codec_codec_manager
#define src_codec_codec_manager

#include "src/task_queue_object.h"

namespace LJMP {

    class CodecManager : public TaskQueueObject {
        disable_copy(CodecManager)

    public:
        using Ptr = std::shared_ptr<CodecManager>;
        static Ptr create(const TaskQueue::Ptr& task_queue);

    public:
        ~CodecManager() override = default;

        virtual bool initialize() = 0;
        virtual void uninitialize() = 0;

    protected:
        explicit CodecManager(const TaskQueue::Ptr& task_queue) : TaskQueueObject(task_queue, false){}
    };
}
#endif // !src_codec_codec_manager

