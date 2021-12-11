#include "src/codec/media_codec_manager.h"

#include <memory>
#include <functional>

#include "src/utils.h"
#include "src/log.h"
#include "src/media.h"
#include "ljmedia/error_code.h"

namespace LJMP {
    CodecManager::Ptr CodecManager::create(const TaskQueue::Ptr& task_queue) {
        return createPtr<Ecodec::MediaEcodecManager>(task_queue);
    }

    namespace Ecodec {
        
        MediaEcodecManager::MediaEcodecManager(const TaskQueue::Ptr& task_queue) : CodecManager(task_queue) {
            LOG_CREATER;
        }

        MediaEcodecManager::~MediaEcodecManager() {
            LOG_DESTRUCT;
        }
    
        bool MediaEcodecManager::initialize() {
            LOG_ENTER;

           
            return true;
        }
    
        void MediaEcodecManager::uninitialize() {
            LOG_ENTER;

            
        }
    
        

    }
}