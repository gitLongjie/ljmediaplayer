#include "src/input/rtmp/rtmp_status.h"

#include "src/log.h"
#include "src/media_flv.h"
#include "src/input/rtmp/rtmp_context.h"

#include "ljmedia/error_code.h"

namespace LJMP {
    namespace Input {
        namespace Rtmp {

            RtmpStatus::RtmpStatus(const RtmpContext::Ptr& rtmp_context)
                : rtmp_context_(rtmp_context) {
                LOGI("actor {}", (long long)this);

            }

            RtmpStatus::~RtmpStatus() {
                LOGI("dctor {}", (long long)this);
            }


            RtmpReaderStatus::RtmpReaderStatus(const RtmpContext::Ptr& rtmp_context) :RtmpStatus(rtmp_context) {

            }

            RtmpReaderStatus::Ptr RtmpReadFLVHeaderStatus::create(const RtmpContext::Ptr& rtmp_context) {
                return createPtr<RtmpReadFLVHeaderStatus>(rtmp_context);
            }

            RtmpReadFLVHeaderStatus::RtmpReadFLVHeaderStatus(const RtmpContext::Ptr& rtmp_context)
                : RtmpReaderStatus(rtmp_context) {

            }

            void RtmpReadFLVHeaderStatus::read(RTMP* rtmp) {
                LOG_ENTER;

                RtmpContext::Ptr context = getRtmpContext();
                if (!context) {
                    LOGE("rtmp context is destruct");
                    return;
                }

                FLVHeader flvHeader;
                const int flv_header_size = sizeof(FLVHeader);
                int ret = RTMP_Read(rtmp, reinterpret_cast<char*>(&flvHeader), flv_header_size);
                if (flv_header_size != ret) {
                    LOGE("read flv header failed, ret={}", ret);
                    context->errorCallback(error_code_read_network_data_failed, "read network data failed");
                    return;
                }
                long long duration = static_cast<long long>(rtmp->m_fDuration * 1000000);
                std::string path(rtmp->Link.playpath.av_val, rtmp->Link.playpath.av_len);
                LOGI("path={}, a={}, v={}, duration={} seconds", path, flvHeader.isContainsAudio(),
                    flvHeader.isContainsVideo(), duration / 1000000);

                context->handleFlvHeader(flvHeader);
                context->switchReaderStatus(RtmpReadFLVTagDataStatus::create(context));
            }

            RtmpReaderStatus::Ptr RtmpReadFLVTagDataStatus::create(const RtmpContext::Ptr& rtmp_context) {
                return createPtr<RtmpReadFLVTagDataStatus>(rtmp_context);
            }

            RtmpReadFLVTagDataStatus::RtmpReadFLVTagDataStatus(const RtmpContext::Ptr& rtmp_context)
                : RtmpReaderStatus(rtmp_context) {

            }

            void RtmpReadFLVTagDataStatus::read(RTMP* rtmp) {
                RtmpContext::Ptr context = getRtmpContext();
                if (!context) {
                    LOGE("rtmp context is destruct");
                    return;
                }

                FLVTagHeader tagHeader;
                const int tag_header_size = sizeof(FLVTagHeader);
                int ret = RTMP_Read(rtmp, reinterpret_cast<char*>(&tagHeader), tag_header_size);
                if (tag_header_size != ret) {
                    LOGE("read tag header failed, ret={}", ret);
                    context->errorCallback(error_code_read_network_data_failed, "read network data failed");
                    return;
                }

                const unsigned int data_size = tagHeader.getTagSize();
                DataBuffer::Ptr data_buffer = DataBuffer::create(data_size);
                ret = RTMP_Read(rtmp, data_buffer->getData(), data_size);
                if (data_size != ret) {
                    LOGE("read flv data failed, ret={}, data size={}", ret, data_size);
                    context->errorCallback(error_code_read_network_data_failed, "read network data failed");
                    return;
                }

                context->handleFlvData(tagHeader, data_buffer);
            }

        }
    }
}