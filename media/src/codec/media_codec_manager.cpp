#include "src/codec/media_codec_manager.h"

#include <memory>
#include <functional>

#include "src/utils.h"
#include "src/log.h"
#include "src/media.h"
#include "ljmedia/error_code.h"

#include "src/codec/media_codec_x264.h"


namespace LJMP {
    MediaCodecManager::Ptr MediaCodecManager::create(const TaskQueue::Ptr& task_queue) {
        return createPtr<Codec::MediaCodecManager>(task_queue);
    }

    static long long codec_index_ = 0;
    long long MediaCodecManager::getCodecIndex() {
        return codec_index_++;
    }

    namespace Codec {
        
        MediaCodecManager::MediaCodecManager(const TaskQueue::Ptr& task_queue) : LJMP::MediaCodecManager(task_queue) {
            LOG_CREATER;
        }

        MediaCodecManager::~MediaCodecManager() {
            LOG_DESTRUCT;
        }
    
        bool MediaCodecManager::initialize() {
            LOG_ENTER;

            CodecType type = MediaCodecX264::getType();
            MediaCodecFactory::Ptr factory = std::make_shared<MediaCodecFactoryImpl<MediaCodecX264> >(type);
            addFactory(factory);

            return true;
        }
    
        void MediaCodecManager::uninitialize() {
            LOG_ENTER;

            
        }

        MediaCodecFactory::Ptr MediaCodecManager::getCodecFactory(CodecType type) const {
            LOG_ENTER;

            const auto itor = factory_list_.find(type);
            if (factory_list_.end() == itor) {
                LOGD("not container codec factory type={}", type);
                return nullptr;
            }
            return itor->second;
        }

        void MediaCodecManager::addFactory(const MediaCodecFactory::Ptr& factory) {
            LOG_ENTER;

            if (!factory) {
                LOGE("factory is nullptr");
                return;
            }
            factory_list_[factory->getCodeType()] = factory;
        }
        
    
        

    }
}
