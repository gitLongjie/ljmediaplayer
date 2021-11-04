#include "src/media.h"

#include <memory>
#include <cassert>

namespace LJMP {
    Media* s_media = nullptr;

    Media::Media() { 
        assert(nullptr == s_media);

        s_media = this;
    }

    Media::~Media() { 
        s_media = nullptr;
    }

    Media* Media::getInstance() {
        return s_media;
    }

    bool Media::openUrl(const char *szUrl) { return false; }
} // namespace LJMP 
