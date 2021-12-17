#ifndef src_media_codec_channel_h_
#define src_media_codec_channel_h_

#include "src/media_channel.h"
#include "src/media_source_channel_data_recive.h"

namespace LJMP {
    class MediaSourceChannel;

    class MediaCodec;

    class MediaCodecChannel : public MediaChannel, public MediaSourceChannelDataRecive {
        disable_copy(MediaCodecChannel)

    public:
        using Ptr = std::shared_ptr<MediaCodecChannel>;
        static Ptr create(const TaskQueue::Ptr& task_queue);

    public:
        ~MediaCodecChannel() override;

        void onReciveScripte(const std::shared_ptr<MediaConfig>& config) override;
        void onReciveVideo(const std::shared_ptr<DataBuffer>& data_buffer) override;

    public:
        void bindMediaSourceChannel(const std::shared_ptr<MediaSourceChannel>& source_channel);

    protected:
        explicit MediaCodecChannel(const TaskQueue::Ptr& task_queue);

        void onHandleScript(const std::shared_ptr<MediaConfig> config, WPtr wThis);
        void onHandleVideo(const std::shared_ptr<DataBuffer> data_buffer, WPtr wThis);

        std::shared_ptr<MediaCodec> createMediaCodec(CodecType type);

    private:
        std::shared_ptr<MediaCodec> video_codec_;
    };
}

#endif // !src_media_codec_channel_h_

