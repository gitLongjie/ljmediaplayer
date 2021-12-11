#ifndef src_media_codec_channel_h_
#define src_media_codec_channel_h_

#include "src/media_channel.h"
#include "src/media_source_channel_data_recive.h"

namespace LJMP {
    class MediaSourceChannel;

    class MediaCodecChannel : public MediaChannel, public MediaSourceChannelDataRecive {
        disable_copy(MediaCodecChannel)

    public:
        using Ptr = std::shared_ptr<MediaCodecChannel>;
        static Ptr create(const TaskQueue::Ptr& task_queue);

    public:
        ~MediaCodecChannel() override;

        void onUpdateScripte(const std::shared_ptr<MediaConfig>& config) override;

    public:
        void bindMediaSourceChannel(const std::shared_ptr<MediaSourceChannel>& source_channel);

    protected:
        explicit MediaCodecChannel(const TaskQueue::Ptr& task_queue);

        void onHandleScript(const std::shared_ptr<MediaConfig> config, WPtr wThis);
    };
}

#endif // !src_media_codec_channel_h_

