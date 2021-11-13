#ifndef src_input_rtsp_media_source_h_
#define src_input_rtsp_media_source_h_

#include "src/media_source.h"
#include "src/media_source_factory.h"

namespace LJMP {
    namespace Input {
        class RTSPMediaSource : public LJMP::MediaSource {
            disable_copy(RTSPMediaSource)

        public:
            static const char* protocol();

        public:
            ~RTSPMediaSource() override;

            bool load(const std::string& url) override;

        protected:
            RTSPMediaSource();

        private:
            friend class MediaSourceFactoryImpl<RTSPMediaSource>;
        };
    }
}

#endif // !src_input_rtsp_media_source_h_
