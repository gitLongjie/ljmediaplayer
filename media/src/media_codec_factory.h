#ifndef src_codec_factory_h_
#define  src_codec_factory_h_

#include <memory>

#include "src/lj_defined.h"
#include "src/task_queue.h"

namespace LJMP {
    class MediaCodec;

    class MediaCodecFactory {

    public:
        using Ptr = std::shared_ptr<MediaCodecFactory>;

    public:
        virtual ~MediaCodecFactory() = default;
        virtual CodecType getCodeType() const = 0;
        virtual std::shared_ptr<MediaCodec> create(const TaskQueue::Ptr& task_queue) = 0;
    };

    template <typename T>
    class MediaCodecFactoryImpl : public MediaCodecFactory {
        disable_copy(MediaCodecFactoryImpl)

    public:
        explicit MediaCodecFactoryImpl(CodecType code_type) :code_type_(code_type){}
        ~MediaCodecFactoryImpl() override {}

        CodecType getCodeType() const override { return code_type_; }
        std::shared_ptr<MediaCodec> create(const TaskQueue::Ptr& task_queue) {
            return createPtr<T>(task_queue);
        }

    private:
        CodecType code_type_;
    };
}

#endif // !src_codec_factory_h_

