#ifndef src_media_source_h_
#define src_media_source_h_

#include <memory>

#include "src/lj_defined.h"

namespace LJMP {
    class MediaSource {
    public:
        virtual ~MediaSource() = default;

        virtual bool load(const char* url) = 0;

    protected:
        MediaSource() = default;
    };

    using MediaSourcePtr = std::shared_ptr<MediaSource>;
} // namespace LJMP


#endif // ! src_media_source_h_
