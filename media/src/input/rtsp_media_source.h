#ifndef src_input_rtsp_media_source_h_
#define src_input_rtsp_media_source_h_

#include "src/media_source.h"
#include "src/media_source_factory.h"

namespace LJMP {
    namespace Input {
        class RTSPMediaSource : public MediaSource {
            disable_copy(RTSPMediaSource)

        public:
            static const StringList& protocols();
            static bool checkProtocol(const std::string& protocol);

        public:
            ~RTSPMediaSource() override;

        protected:
            RTSPMediaSource();

            bool doOpen(const std::string& url) override;
            void doClose() override;
        };
    }
}

#endif // !src_input_rtsp_media_source_h_
