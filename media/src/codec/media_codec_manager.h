#ifndef src_codec_medai_codec_manager_h_
#define src_codec_medai_codec_manager_h_

#include <memory>
#include <list>
#include <map>
#include <string>

#include "src/codec_manager.h"
#include "src/media_source_factory.h"

namespace LJMP {
    namespace Ecodec{

        class MediaEcodecManager : public CodecManager {
            disable_copy(MediaEcodecManager)

        public:
            ~MediaEcodecManager() override;
            
            bool initialize() override;
            void uninitialize() override;
            
        protected:
            explicit MediaEcodecManager(const TaskQueue::Ptr& ptr);

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

#endif // !src_codec_medai_codec_manager_h_
