#include "src/codec/media_codec_ffmpeg_decode.h"

#include "src/core/log.h"
#include "src/codec/codec_utils.h"

namespace LJMP {
    namespace Codec {

        MediaCodecFFmpegDecode::MediaCodecFFmpegDecode(const TaskQueue::Ptr& task_queue)
            : MediaCodec(task_queue) {
            LOG_CREATER;
        }

        MediaCodecFFmpegDecode::~MediaCodecFFmpegDecode() {
            LOG_DESTRUCT;
        }

        CodecType MediaCodecFFmpegDecode::getType() {
            return CodecType::Video_FFMpeg_Decode;
        }

        bool MediaCodecFFmpegDecode::onInitialize(const std::shared_ptr<MediaConfig>& config) {
            LOG_ENTER;

            if (initialized_) {
                LOGI("aleary is initialzed");
                return true;
            }

            config_ = std::make_shared<MediaConfig>(*config.get());

            initialized_ = true;

            return true;
        }

        void MediaCodecFFmpegDecode::onUninitialize() {
            LOG_ENTER;

            initialized_ = false;
            uninitCodecContext();

            if (nullptr != av_packet_) {
                av_packet_free(&av_packet_);
            }
        }

        void MediaCodecFFmpegDecode::onHandleDataBuffer(const DataBuffer::Ptr& data_buffer) {
            if (!initialized_) {
                LOGE("is not initialize");
                return;
            }

            if (!codec_context_initialize_) {
                initCodecContext(data_buffer);
                return;
            }

            av_packet_->stream_index = 1;
            av_packet_->flags = 0;

            av_packet_->data = reinterpret_cast<unsigned char*>(data_buffer->getData());
            av_packet_->size = data_buffer->getSize() ;
            
            av_packet_->dts = data_buffer->timeStamp();
            av_packet_->pts = data_buffer->timeStampEx();

            int ret = avcodec_send_packet(codec_context_, av_packet_);
            if (0 != ret) {
                if (AVERROR(EAGAIN) != ret) {
                    return;
                }
                LOGE("avcodec send packet failed");
                return;
            }

            do {
                ret = avcodec_receive_frame(codec_context_, av_frame_);
                if (AVERROR_EOF == ret) {
                   break;
                }
                if (0 <= ret) {
                    handleAFrame();
                }
               
               av_frame_unref(av_frame_);
            } while (AVERROR(EAGAIN) != ret);
        }

        bool MediaCodecFFmpegDecode::initCodecContext(const DataBuffer::Ptr& data_buffer) {
            LOG_ENTER;
            
            if (!config_) {
                LOGE("config is nullptr");
                return false;
            }

            if (nullptr != codec_context_) {
                LOGD("codec context is not nullptr");
                return true;
            }

            av_packet_ = av_packet_alloc();
            if (nullptr == av_packet_) {
                LOGE("alloc av packet failed");
                uninitCodecContext();
                return false;
            }

            AVCodecID codec_id = CodecUtils::getAVCodeId(static_cast<CodecType>(config_->vidoe_codec_id));
            if (AV_CODEC_ID_NONE == codec_id) {
                LOGE("codec id not find id={}", config_->vidoe_codec_id);
                uninitCodecContext();
                return false;
            }

            AVCodec* codec = avcodec_find_decoder(codec_id);
            if (nullptr == codec) {
                LOGE("not find codec");
                uninitCodecContext();
                return false;
            }

            codec_context_ = avcodec_alloc_context3(codec);
            if (nullptr == codec_context_) {
                LOGE("avcodec alloc context3 failed");
                uninitCodecContext();
                return false;
            }
            codec_context_->time_base.den = 1;
            codec_context_->time_base.num = 1;

            codec_context_->extradata_size = data_buffer->getSize();
            codec_context_->extradata = (uint8_t*)av_malloc(codec_context_->extradata_size);
            memcpy(codec_context_->extradata, data_buffer->getData(),
                codec_context_->extradata_size);

            codec_context_->codec_type = AVMEDIA_TYPE_VIDEO;
            codec_context_->codec_id = codec_id;
            codec_context_->codec_tag = 0;
            codec_context_->bit_rate = 0;
            codec_context_->bits_per_coded_sample = 0;
            codec_context_->bits_per_raw_sample = 0;

            //codec_context_->get_buffer2()
            codec_context_->thread_type = FF_THREAD_SLICE | FF_THREAD_FRAME;
            codec_context_->width = config_->video_width;
            codec_context_->height = config_->video_height;
            // codec_context_->opaque = this;
            // codec_context_->bit_rate = metaData.getVideoBitrate();

            int ret = avcodec_open2(codec_context_, codec, nullptr);
            if (0 != ret) {
                LOGE("avcodec open failed ret={}", ret);
                uninitCodecContext();
                return false;
            }
           
            av_frame_ = av_frame_alloc();
            if (nullptr == av_frame_) {
                LOGE("avcodec frame failed");
                uninitCodecContext();
                return false;
            }

            codec_context_initialize_ = true;
            return true;
        }

        void MediaCodecFFmpegDecode::uninitCodecContext() {
            LOG_ENTER;

            if (nullptr != av_frame_) {
                av_frame_free(&av_frame_);
                av_frame_ = nullptr;
            }

            if (nullptr != codec_context_) {
                avcodec_close(codec_context_);
                avcodec_free_context(&codec_context_);
                codec_context_ = nullptr;
            }
        }

        void MediaCodecFFmpegDecode::handleAFrame() {
            av_frame_->pts = av_frame_->best_effort_timestamp;
        }

    }
}