#ifndef src_media_source_h_
#define src_media_source_h_

#include "src/lj_defined.h"

namespace LJMP {
    class MediaSource {
    public:
        MediaSource() = default;
        virtual ~MediaSource() = default;

        virtual bool load(const char* url) = 0;
    };
} // namespace LJMP


#endif // ! src_media_source_h_
