#ifndef src_input_rtmp_context_h
#define src_input_rtmp_context_h

#include <memory>
#include <string>

#include "src/lj_defined.h"

#include "src/media_flv.h"
#include "src/network/channel.h"

struct RTMP;

namespace LJMP {
    class MediaSource;

    namespace Input {
        namespace Rtmp {

            class RtmpReaderStatus;

            class RtmpContext : public std::enable_shared_from_this<RtmpContext> {
                disable_copy(RtmpContext)

            public:
                using Ptr = std::shared_ptr<RtmpContext>;
                using WPtr = std::weak_ptr<RtmpContext>;

                static std::shared_ptr<RtmpContext> create(std::weak_ptr<MediaSource> media_source,
                    const std::string& url);

            public:
                virtual ~RtmpContext();

                bool intialize();
                void uninitialzie();

                void errorCallback(int code, const char* msg);
                void switchReaderStatus(std::shared_ptr<RtmpReaderStatus> status) { rtmp_reader_status_ = status; }

                void handleFlvHeader(const FLVHeader& header);
                void handleFlvData(const FLVTagHeader& tagHeader, const DataBuffer::Ptr& data_buffer);

            protected:
                explicit RtmpContext(std::weak_ptr<MediaSource> media_source, const std::string& url);

                void reset();

                void doConnectServer(std::string url, WPtr wThis);
                void doConnected(std::shared_ptr<RTMP> rtmp, WPtr wThis);

                void readDataCallback(const Network::Channel::Ptr& channel, WPtr wThis);
                void doUpdateMedia(bool audio, bool video, WPtr wThis);
                void doHandleFlvData(FLVType flv_type, DataBuffer::Ptr data_buffer, WPtr wThis);

                void doHandleScrpite(const DataBuffer::Ptr& dadata_bufferta);

            private:
                std::weak_ptr<MediaSource> media_source_;
                std::string url_;

                Network::Channel::Ptr channel_;
                std::shared_ptr<RTMP> rtmp_;

                std::shared_ptr<RtmpReaderStatus> rtmp_reader_status_;
                bool is_containe_audio_ = false;
                bool is_containe_video_ = false;
            };
        }
    }
}

#endif // !src_input_rtmp_context_h
