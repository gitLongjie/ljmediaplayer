#ifndef src_input_rtmp_context_h
#define src_input_rtmp_context_h

#include <memory>
#include <string>

#include "src/lj_defined.h"
#include "src/core/task_object.h"

#include "src/media_flv.h"

struct RTMP;

namespace LJMP {
    namespace Input {
        class RTMPMediaSource;
        namespace Rtmp {

            class RtmpReaderStatus;

            class RtmpContext : public TaskObject {
                disable_copy(RtmpContext)

            public:
                using Ptr = std::shared_ptr<RtmpContext>;

                static std::shared_ptr<RtmpContext> create(const TaskQueue::Ptr& task_queue,
                    std::weak_ptr<RTMPMediaSource> media_source, const std::string& url);

            public:
                virtual ~RtmpContext();

                bool intialize();
                void uninitialzie();

                void errorCallback(int code, const char* msg);
                void switchReaderStatus(std::shared_ptr<RtmpReaderStatus> status) { rtmp_reader_status_ = status; }

                void handleFlvHeader(const FLVHeader& header);
               // void handleFlvData(const FLVTagHeader& tagHeader, const DataBuffer::Ptr& data_buffer);

            protected:
                RtmpContext(const TaskQueue::Ptr& task_queue, std::weak_ptr<RTMPMediaSource> media_source,
                    const std::string& url);

                void reset();

                void doConnectServer(std::string url, WPtr wThis);
                void doConnected(std::shared_ptr<RTMP> rtmp, WPtr wThis);

                void readDataCallback(WPtr wThis);
                void doUpdateMedia(bool audio, bool video, WPtr wThis);
               // void doHandleFlvData(FLVType flv_type, DataBuffer::Ptr data_buffer, WPtr wThis);

              /*  void doHandleScrpite(const DataBuffer::Ptr& data_buffer);
                void doHandleVideo(const DataBuffer::Ptr& data_buffer);*/

            private:
                std::weak_ptr<RTMPMediaSource> media_source_;
                std::string url_;

                std::shared_ptr<RTMP> rtmp_;

                std::shared_ptr<RtmpReaderStatus> rtmp_reader_status_;
                bool is_containe_audio_ = false;
                bool is_containe_video_ = false;
            };
        }
    }
}

#endif // !src_input_rtmp_context_h
