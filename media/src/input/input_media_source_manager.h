#ifndef src_input_input_medai_source_h_
#define src_input_input_medai_source_h_

#include <memory>
#include <unordered_map>
#include <string>

#include "src/media_source_manager.h"
#include "src/media_source_factory.h"

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
            
            bool open(const std::string& url) override;
            
        protected:
            InputMediaSourceManager();

        private:
            using MediaSourceFactoryList = std::unordered_map<std::string, MediaSourceFactoryPtr >;
            MediaSourceFactoryList media_source_factory_;
        };
    }
}

#endif // !src_input_input_medai_source_h_
