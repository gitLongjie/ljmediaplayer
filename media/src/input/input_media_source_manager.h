#ifndef src_input_input_medai_source_h_
#define src_input_input_medai_source_h_

#include <memory>

#include "src/media_source_manager.h"

namespace LJMP {
    namespace Input{
        class InputMediaSourceManager : public MediaSourceManager {
            disable_copy(InputMediaSourceManager)
            
        public:
            static std::shared_ptr<InputMediaSourceManager> create();

        public:
            ~InputMediaSourceManager() override;
            
            bool initialize() override;
            void uninitialize() override;
            
            bool open(const char* szUrl);
            
        protected:
            InputMediaSourceManager();
        };
    }
}

#endif // !src_input_input_medai_source_h_
