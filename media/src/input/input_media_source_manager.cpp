#include "src/input/input_media_source_manager.h"

#include <memory>

#include "src/utils.h"
#include "src/input/rtsp_media_source.h"
#include "src/log.h"

namespace LJMP {
    namespace Input {
        InputMediaSourceManager::InputMediaSourceManager() {
            LOGI("actor {}", (long long)this);
        }
        
        InputMediaSourceManager::~InputMediaSourceManager() { 
            LOGI("dactor {}", (long long)this);
        }
    
        std::shared_ptr<InputMediaSourceManager> InputMediaSourceManager::create() {
            struct InputMediaSourceManagerCreator : public InputMediaSourceManager {
                InputMediaSourceManagerCreator() : InputMediaSourceManager() {}
                ~InputMediaSourceManagerCreator() = default;
            };
            
            return std::make_shared<InputMediaSourceManagerCreator>();
        }
    
        bool InputMediaSourceManager::initialize() {
            LOG_ENTER;

            MediaSourceFactoryPtr ptr = std::make_shared<MediaSourceFactoryImpl<RTSPMediaSource> >();
            media_source_factory_[RTSPMediaSource::protocol()] = ptr;
            return true;
        }
    
        void InputMediaSourceManager::uninitialize() {
            LOG_ENTER;

            media_source_factory_.clear();
        }
    
        bool InputMediaSourceManager::open(const std::string& url) {
            LOG_ENTER;

            std::string protocol = Utils::getProtocol(url);
            auto itor = media_source_factory_.find(protocol);
            if (media_source_factory_.end() == itor) {
                LOGE("con't not find protocol {}", protocol);
                return false;
            }

            MediaSourcePtr ptr = itor->second->create();
            if (!ptr) {
                LOGE("con't create media source");
                return false;
            }
            
            return ptr->load(url);
        }
    }
}
