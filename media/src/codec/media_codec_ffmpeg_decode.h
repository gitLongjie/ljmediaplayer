#ifndef src_media_codec_ffmpeg_decode_h_
#define src_media_codec_ffmpeg_decode_h_

#ifdef __cplusplus
extern "C" {
#endif

#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>

#ifdef __cplusplus
}
#endif

#include "src/media_codec.h"



namespace LJMP {
    namespace Codec {
        class MediaCodecFFmpegDecode : public MediaCodec {
            disable_copy(MediaCodecFFmpegDecode)

        public:
            static CodecType getType();

        public:
            ~MediaCodecFFmpegDecode() override;

        protected:
            explicit MediaCodecFFmpegDecode(const TaskQueue::Ptr& task_queue);

        protected:
            bool onInitialize(const std::shared_ptr<MediaConfig>& config) override;
            void onUninitialize() override;
            void onHandleDataBuffer(const DataBuffer::Ptr& data_buffer) override;

        private:
            bool initCodecContext(const DataBuffer::Ptr& data_buffer);
            void uninitCodecContext();
            void handleAFrame();

        private:
            std::shared_ptr<MediaConfig> config_;
            AVCodecContext* codec_context_ = nullptr;
            bool codec_context_initialize_ = false;
            AVPacket* av_packet_ = nullptr;
            bool initialized_ = false;

            AVFrame *av_frame_ = nullptr;
            
        };
    }
}

#endif // !src_media_codec_ffmpeg_decode_h_
