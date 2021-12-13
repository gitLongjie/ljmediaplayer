#include "src/codec/media_codec_manager.h"

#include <memory>
#include <functional>

#include "src/utils.h"
#include "src/log.h"
#include "src/media.h"
#include "ljmedia/error_code.h"

#include "src/codec/media_codec_x264.h"


namespace LJMP {
    CodecManager::Ptr CodecManager::create(const TaskQueue::Ptr& task_queue) {
        return createPtr<Codec::MediaCodecManager>(task_queue);
    }

    namespace Codec {
        
        MediaCodecManager::MediaCodecManager(const TaskQueue::Ptr& task_queue) : CodecManager(task_queue) {
            LOG_CREATER;
        }

        MediaCodecManager::~MediaCodecManager() {
            LOG_DESTRUCT;
        }
    
        bool MediaCodecManager::initialize() {
            LOG_ENTER;

            CodecType type = MediaCodecX264::getType();
            CodecFactory::Ptr factory = std::make_shared<CodecFactoryImpl<MediaCodecX264> >(type);
            addFactory(factory);

            return true;
        }
    
        void MediaCodecManager::uninitialize() {
            LOG_ENTER;

            
        }

        CodecFactory::Ptr MediaCodecManager::getCodecFactory(CodecType type) const {
            LOG_ENTER;

            const auto itor = factory_list_.find(type);
            if (factory_list_.end() == itor) {
                LOGD("not container codec factory type={}", type);
                return nullptr;
            }
            return itor->second;
        }

        void MediaCodecManager::addFactory(const CodecFactory::Ptr& factory) {
            LOG_ENTER;

            if (!factory) {
                LOGE("factory is nullptr");
                return;
            }
            factory_list_[factory->getCodeType()] = factory;
        }
        
    
        

    }
}
