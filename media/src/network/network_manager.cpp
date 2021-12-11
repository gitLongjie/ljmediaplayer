#include "src/input/input_media_source_manager.h"

#include <memory>
#include <functional>

#include "src/utils.h"
#include "src/input/rtsp_media_source.h"
#include "src/input/rtmp_media_source.h"
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

            MediaSourceFactoryPtr ptr = std::make_shared<MediaSourceFactoryImpl<RTSPMediaSource, RTSPMediaSource::checkProtocol> >();
            media_source_factory_.emplace_back(ptr);
            ptr = std::make_shared<MediaSourceFactoryImpl<RTMPMediaSource, RTMPMediaSource::checkProtocol> >();
            media_source_factory_.emplace_back(ptr);
            return true;
        }
    
        void InputMediaSourceManager::uninitialize() {
            LOG_ENTER;

            media_source_factory_.clear();
        }
    
        bool InputMediaSourceManager::open(const std::string& url) {
            LOG_ENTER;

            std::string protocol = Utils::getProtocol(url);
            auto itor = std::find_if(media_source_factory_.begin(), media_source_factory_.end(),
                [=](const auto& item)->bool {
                    return item->isSupportProtocol(protocol);
                }
            );
            if (media_source_factory_.end() == itor) {
                LOGE("con't not find protocol {}", protocol);
                return false;
            }

            MediaSourcePtr ptr = (*itor)->create();
            if (!ptr) {
                LOGE("con't create media source");
                return false;
            }
            
            return true; // ptr->load(url);
        }
    }
}
