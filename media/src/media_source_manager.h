#ifndef src_media_source_manager_h_
#define src_media_source_manager_h_

#include <memory>
#include <string>

#include "src/lj_defined.h"

namespace LJMP {
    class MediaSource;

    class MediaSourceManager {
    public:
        virtual ~MediaSourceManager() = default;

        virtual bool initialize() = 0;
        virtual void uninitialize() = 0;

        virtual std::shared_ptr<MediaSource> getMediaSource(const std::string& url) = 0;
        //virtual bool open(const std::string& url) = 0;
    };
}

#endif  // !src_media_source_manager_h_
