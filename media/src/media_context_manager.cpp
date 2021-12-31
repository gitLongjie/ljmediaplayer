#include "src/media_context_manager.h"

#include "src/core/log.h"
#include "src/media_context.h"

namespace LJMP {

    MediaContextManager::Ptr MediaContextManager::create() {
        return createPtr<MediaContextManager>();
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

    void MediaContextManager::destroy() {
        for (auto& item : media_context_url_lists_) {
            item.second->destory();
        }
    }

}
