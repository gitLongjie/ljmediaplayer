#include "src/media_context.h"

namespace LJMP {

    MediaContext::Ptr MediaContext::create() {
        struct Creator : public MediaContext {
            Creator() : MediaContext() {}
            ~Creator() override = default;
        };

        return std::make_shared<Creator>();
    }

}
