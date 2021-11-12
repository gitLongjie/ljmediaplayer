#include "src/input/input_media_source_manager.h"

#include "src/utils.h"

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
            return true;
        }
    
        void InputMediaSourceManager::uninitialize() {
            
        }
    
        bool InputMediaSourceManager::open(const char *szUrl) {
            if (Utils::isFileExits(szUrl)) {
                return false;
            }
            
            return true;
        }
    }
}
