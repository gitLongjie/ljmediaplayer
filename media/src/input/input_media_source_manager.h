#ifndef src_input_input_medai_source_h_
#define src_input_input_medai_source_h_

#include <memory>
#include <list>
#include <map>
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
            
            MediaSource::Ptr getMediaSource(const std::string& url) override;
            
        protected:
            InputMediaSourceManager();

            void addMediaSource(const std::string& url, const MediaSource::Ptr& media_source);
            void removeMediaSource(const std::string& url);

        private:
            using MediaSourceFactoryList = std::list<MediaSourceFactoryPtr >;
            using MediaSourceList = std::map<std::string, MediaSource::Ptr>;
            MediaSourceFactoryList media_source_factory_;
            MediaSourceList media_sources_;
        };
    }
}

#endif // !src_input_input_medai_source_h_
