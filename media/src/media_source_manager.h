#ifndef src_media_source_manager_h_
#define src_media_source_manager_h_

#include "src/lj_defined.h"

namespace LJMP {
    class MediaSourceManager {
    public:
        virtual ~MediaSourceManager() = default;

        virtual bool initialize() = 0;
        virtual void uninitialize() = 0;
    };
}

#endif  // !src_media_source_manager_h_
