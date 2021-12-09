#ifndef src_media_context_h_
#define src_media_context_h_

#include <memory>

#include "src/lj_defined.h"

namespace LJMP {
    class MediaSource;

    class MediaContext : public std::enable_shared_from_this<MediaContext> {
        disable_copy(MediaContext)

    public:
        using Ptr = std::shared_ptr<MediaContext>;
        using WPtr = std::weak_ptr<MediaContext>;

        static Ptr create();
    public:
        virtual ~MediaContext() = default;

        void updateMediaSrouce(const std::shared_ptr<MediaSource>& media_source) {
            media_source_ = media_source;
        }

    protected:
        MediaContext() = default;

    private:
        std::shared_ptr<MediaSource> media_source_;
    };
}

#endif // !src_media_context_h_

