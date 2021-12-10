#include "src/input/input_media_source_manager.h"

#include <memory>
#include <functional>

#include "src/utils.h"
#include "src/input/rtsp_media_source.h"
#include "src/input/rtmp_media_source.h"
#include "src/log.h"
#include "src/media.h"
#include "ljmedia/error_code.h"

namespace LJMP {
    namespace Input {
        std::shared_ptr<InputMediaSourceManager> InputMediaSourceManager::create() {
            return createPtr<InputMediaSourceManager>();
        }

        InputMediaSourceManager::InputMediaSourceManager() {
            LOGI("actor {}", (long long)this);
        }

        InputMediaSourceManager::~InputMediaSourceManager() {
            LOGI("dactor {}", (long long)this);
        }
    
        bool InputMediaSourceManager::initialize() {
            LOG_ENTER;

            MediaSourceFactoryPtr ptr = std::make_shared<MediaSourceFactoryImpl<RTSPMediaSource, RTSPMediaSource::checkProtocol> >();
            media_source_factory_.emplace_back(ptr);
            ptr = std::make_shared<MediaSourceFactoryImpl<RTMPMediaSource, RTMPMediaSource::checkProtocol> >();
            media_source_factory_.emplace_back(ptr);
            return true;
        }
    
        void InputMediaSourceManager::uninitialize() {
            LOG_ENTER;

            for (const auto& item : media_sources_) {
                item.second->stop();
            }
            media_sources_.clear();
            media_source_factory_.clear();
        }
    
        MediaSource::Ptr InputMediaSourceManager::getMediaSource(const std::string& url) {
            std::string protocol = Utils::getProtocol(url);
            auto itor = std::find_if(media_source_factory_.begin(), media_source_factory_.end(),
                [=](const auto& item)->bool {
                    return item->isSupportProtocol(protocol);
                }
            );
            if (media_source_factory_.end() == itor) {
                LOGE("con't not find protocol {}", protocol);
                return nullptr;
            }


            MediaSource::Ptr media_source = (*itor)->create(url);
            if (!media_source) {
                LOGE("con't create media source");
                return nullptr;
            }
            return media_source;
        }
// 
//             if (!ptr->open(url)) {
//                 Media::getInstance()->errorCallbak(error_code_open_failed, "can't open url=" + url);
//                 return false;
//             }
// 
//         }
// 
//         bool InputMediaSourceManager::open(const std::string& url) {
//             LOG_ENTER;
// 
//             std::string protocol = Utils::getProtocol(url);
//             auto itor = std::find_if(media_source_factory_.begin(), media_source_factory_.end(),
//                 [=](const auto& item)->bool {
//                     return item->isSupportProtocol(protocol);
//                 }
//             );
//             if (media_source_factory_.end() == itor) {
//                 LOGE("con't not find protocol {}", protocol);
//                 return false;
//             }
// 
//             
//             MediaSource::Ptr ptr = (*itor)->create();
//             if (!ptr) {
//                 LOGE("con't create media source");
//                 return false;
//             }
// 
//             if (!ptr->open(url)) {
//                 Media::getInstance()->errorCallbak(error_code_open_failed, "can't open url=" + url);
//                 return false;
//             }
//             
//             addMediaSource(url, ptr);
//             return true;
//         }

        void InputMediaSourceManager::addMediaSource(const std::string& url, const MediaSource::Ptr& media_source) {
            LOG_ENTER;

            media_sources_[url] = media_source;
        }

        void InputMediaSourceManager::removeMediaSource(const std::string& url) {
            LOG_ENTER;

            media_sources_.erase(url);
        }

    }
}
