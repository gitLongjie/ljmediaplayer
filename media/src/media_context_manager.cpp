#include "src/media_context_manager.h"

#include "src/log.h"
#include "src/media_context.h"

namespace LJMP {

    MediaContextManager::Ptr MediaContextManager::create() {
        struct Creator : public MediaContextManager {
            Creator() : MediaContextManager() {}
            ~Creator() override = default;
        };

        return std::make_shared<Creator>();
    }

    bool MediaContextManager::addMediaContext(const MediaContext::Ptr& media_contxt, const std::string& url) {
        if (isContain(url)) {
            return true;
        }

        LOGI("add url={}", url);
        media_context_url_lists_[url] = media_contxt;
        return true;
    }

    void MediaContextManager::removeMediaContext(const std::string& url) {
        LOGI("remove url={}", url);
        if (!isContain(url)) {
            return ;
        }

        media_context_url_lists_.erase(url);
    }

}
