#ifndef src_input_rtmp_media_source_h_
#define src_input_rtmp_media_source_h_

#include "src/media_source.h"
#include "src/media_source_factory.h"

#include "src/input/rtmp/rtmp_context.h"

namespace LJMP {
    namespace Input {
        class RTMPMediaSource : public MediaSource {
            disable_copy(RTMPMediaSource)

        public:
            static const StringList& protocols();
            static bool checkProtocol(const std::string& protocol);

        public:
            ~RTMPMediaSource() override;

            void errorCallback(int, const char*);

            void onHandleScripte(const DataBuffer::Ptr& data_buffer);

        protected:
            explicit RTMPMediaSource(const std::string& url);

            bool doOpen(const std::string& url);
            void doClose();

            void doErrorCallback(int code, std::string msg, WPtr wThis);

        private:
            Rtmp::RtmpContext::Ptr rtmp_context_;
        };
    }
}

#endif // !src_input_rtmp_media_source_h_
