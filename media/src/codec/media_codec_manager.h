#ifndef src_codec_medai_codec_manager_h_
#define src_codec_medai_codec_manager_h_

#include <memory>
#include <map>
#include <string>

#include "src/media_codec_manager.h"

namespace LJMP {
    namespace Codec{

        class MediaCodecManager : public LJMP::MediaCodecManager {
            disable_copy(MediaCodecManager)

        public:
            ~MediaCodecManager() override;
            
            bool initialize() override;
            void uninitialize() override;

            MediaCodecFactory::Ptr getCodecFactory(CodecType type) const override;
            
        protected:
            explicit MediaCodecManager(const TaskQueue::Ptr& ptr);

        private:
            void addFactory(const MediaCodecFactory::Ptr& factory);

        private:
            using FactoryLists = std::map<CodecType, MediaCodecFactory::Ptr >;
            FactoryLists factory_list_;
        };
    }
}

#endif // !src_codec_medai_codec_manager_h_
