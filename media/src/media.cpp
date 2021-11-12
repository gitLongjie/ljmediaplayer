#include "src/media.h"

#include <memory>
#include <cassert>

#include "src/utils.h"

#include "src/log.h"
#include "src/input/input_media_source_manager.h"

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

    bool Media::openUrl(const char *szUrl) {
        std::shared_ptr<Input::InputMediaSourceManager> media_source_manager =
        std::dynamic_pointer_cast<Input::InputMediaSourceManager>(input_media_source_manager_);
        if (media_source_manager) {
            return media_source_manager->open(szUrl);
        }
        return false;
    }

    bool Media::initialize() {
        log_ = Log::create();
        if (!log_) {
            return false;
        }
        
        LOG_ENTER;
        input_media_source_manager_ = Input::InputMediaSourceManager::create();
        if (!input_media_source_manager_->initialize()) {
            return false;
        }
        LOG_LIVE;
        return true;
    }

    void Media::uninitialize() {
        LOG_ENTER;
        input_media_source_manager_.reset();
        
        LOG_LIVE;
        log_.reset();
    }

} // namespace LJMP 
