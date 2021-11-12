#include "src/input/input_media_source_manager.h"

#include <memory>

#include "src/utils.h"
#include "src/input/rtsp_media_source.h"

namespace LJMP {
    namespace Input {
        InputMediaSourceManager::InputMediaSourceManager() {}
        
        InputMediaSourceManager::~InputMediaSourceManager() { }
    
        std::shared_ptr<InputMediaSourceManager> InputMediaSourceManager::create() {
            struct InputMediaSourceManagerCreator : public InputMediaSourceManager {
                InputMediaSourceManagerCreator() : InputMediaSourceManager() {}
                ~InputMediaSourceManagerCreator() = default;
            };
            
            return std::make_shared<InputMediaSourceManagerCreator>();
        }
    
        bool InputMediaSourceManager::initialize() {
            MediaSourceFactoryPtr ptr = std::make_shared<MediaSourceFactoryImpl<RTSPMediaSource> >();
            return true;
        }
    
        void InputMediaSourceManager::uninitialize() {
            
        }
    
        bool InputMediaSourceManager::open(const std::string& url) {
            if (Utils::isFileExits(url.c_str())) {
                return false;
            }
            
            return true;
        }
    }
}
