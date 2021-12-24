#include "src/input/rtmp/rtmp_context.h"

#include <functional>

#include "ljmedia/error_code.h"
#include "src/log.h"
#include "src/media_flv.h"
#include "src/network/socket.h"
#include "src/media.h"
#include "src/thread_pools.h"

#include "src/utils.h"
#include "src/media_source.h"

#include "src/input/rtmp_media_source.h"
#include "src/input/rtmp/rtmp.h"
#include "src/input/rtmp/rtmp_status.h"
#include "src/input/rtmp/rtmp_utils.h"


namespace LJMP {
    namespace Input {
        namespace Rtmp {

            void destroyRtmp(RTMP* r) {
                LOGI("destory rtmp {}", long long(r));
                if (nullptr != r) {
                    if (RTMP_IsConnected(r)) {
                        RTMP_Close(r);
                    }
                    RTMP_Free(r);
                }
            }

            RtmpContext::Ptr RtmpContext::create(const TaskQueue::Ptr& task_queue,
                std::weak_ptr<RTMPMediaSource> media_source, const std::string& url) {
                return createPtr<RtmpContext>(task_queue, media_source, url);
            }

            RtmpContext::RtmpContext(const TaskQueue::Ptr& task_queue,
                std::weak_ptr<RTMPMediaSource> media_source, const std::string& url)
                : TaskQueueObject(task_queue, false)
                , media_source_(media_source)
                , url_(url){
                LOGI("actor {} url={}", (long long)this, url);

            }
            
            RtmpContext::~RtmpContext() {
                LOGI("dctor {}", (long long)this);
            }

            bool RtmpContext::intialize() {
                LOG_ENTER;

                reset();

                ThreadPoolPtr threadPool = Media::getInstance()->getThreadPool();
                if (!threadPool) {
                    LOGE("thread pool is nullptr");
                    return false;
                }

                WPtr wThis(shared_from_this());
                threadPool->pushTask(std::bind(&RtmpContext::doConnectServer, this, url_, wThis));
                return true;
            }

            void RtmpContext::uninitialzie() {
                LOG_ENTER;

                if (channel_) {
                    channel_->disconnect();
                    channel_.reset();
                }

            }

            void RtmpContext::reset() {
               
            }

            void RtmpContext::doConnectServer(std::string url, WPtr wThis) {
                LOG_ENTER;

                if (url.empty()) {
                    LOGD("url is nullptr");
                    return;
                }

                TaskQueueObject::Ptr self(wThis.lock());
                if (!self) {
                    LOGE("rtmp context is desturct {}", (long long)this);
                    return;
                }

                std::shared_ptr<RTMP> rtmp(RTMP_Alloc(), destroyRtmp);
                LOGI("create rtmp {}", long long(rtmp.get()));
                RTMP_Init(rtmp.get());

                if (0 == RTMP_SetupURL(rtmp.get(), const_cast<char*>(url.c_str()))) {
                    errorCallback(error_code_netork_initialize_failed, "setup url failed");
                    LOGE("rtmp setup url failed");
                    return;
                }

                if (0 == RTMP_Connect(rtmp.get(), nullptr)) {
                    errorCallback(error_code_netork_connect_failed, "connect server failed");
                    LOGE("connect server failed");
                    return;
                }

                auto task = createTask(std::bind(&RtmpContext::doConnected, this, rtmp, wThis));
                invoke(task);
            }

            void RtmpContext::doConnected(std::shared_ptr<RTMP> rtmp, WPtr wThis) {
                LOG_ENTER;

                TaskQueueObject::Ptr self(wThis.lock());
                if (!self) {
                    LOGE("this object is destructed");
                    return;
                }

                rtmp_ = std::move(rtmp);

                rtmp_reader_status_ = RtmpReadFLVHeaderStatus::create(std::dynamic_pointer_cast<RtmpContext>(self));

                Network::Socket::Ptr socket = Network::Socket::create(rtmp_->m_sb.sb_socket, Network::Socket::Model::TCP);
                //socket->enableBlock(false);
                channel_ = Network::Channel::create(Media::getInstance()->getIOTaskQueue(), socket);
                channel_->setReadCallbackHandle(std::bind(&RtmpContext::readDataCallback, this, wThis));
            }

            void RtmpContext::errorCallback(int code, const char* msg) {
                LOG_ENTER;

                MediaSource::Ptr media_source(media_source_.lock());
                if (!media_source) {
                    LOGE("media source is destruct");
                    return;
                }

                std::shared_ptr<RTMPMediaSource> source = std::dynamic_pointer_cast<RTMPMediaSource>(media_source);
                if (!source) {
                    LOGE("source is not rtmp media source");
                    return;
                }
                source->errorCallback(code, msg);
            }

            void RtmpContext::handleFlvHeader(const FLVHeader& header) {
                MediaSource::Ptr media_source(media_source_.lock());
                if (!media_source) {
                    LOGE("media source is destruct");
                    return;
                }

                WPtr wThis(shared_from_this());
                auto task = createTask(std::bind(&RtmpContext::doUpdateMedia, this, header.isContainsAudio(),
                    header.isContainsVideo(), wThis));

                invoke(task);
            }

            void RtmpContext::handleFlvData(const FLVTagHeader& tagHeader, const DataBuffer::Ptr& data_buffer) {
                MediaSource::Ptr media_source(media_source_.lock());
                if (!media_source) {
                    LOGE("media source is destruct");
                    return;
                }

                WPtr wThis(shared_from_this());
                auto task = createTask(std::bind(&RtmpContext::doHandleFlvData, this, FLVType(tagHeader.type), data_buffer, wThis));
                invoke(task);
            }

            void RtmpContext::readDataCallback(WPtr wThis) {
                TaskQueueObject::Ptr self(wThis.lock());
                if (!self) {
                    return;
                }
                rtmp_reader_status_->read(rtmp_.get());
            }

            void RtmpContext::doUpdateMedia(bool audio, bool video, WPtr wThis) {
                TaskQueueObject::Ptr self(wThis.lock());
                if (!self) {
                    return;
                }
                is_containe_audio_ = audio;
                is_containe_video_ = video;
            }

            void RtmpContext::doHandleFlvData(FLVType flv_type, DataBuffer::Ptr data_buffer, WPtr wThis) {
                TaskQueueObject::Ptr self(wThis.lock());
                if (!self) {
                    return;
                }
                
                switch (flv_type) {
                case FLVType::SCRIPTE:
                    doHandleScrpite(data_buffer);
                    break;
                case FLVType::AUDIO:

                    break;
                case FLVType::VIDEO:
                    doHandleVideo(data_buffer);
                    break;
                default:
                    break;
                }
            }

            void RtmpContext::doHandleScrpite(const DataBuffer::Ptr& data_buffer) {
                RTMPMediaSource::Ptr media_source(media_source_.lock());
                if (!media_source) {
                    LOG_DESTRUCT;
                    return;
                }

                media_source->onHandleScripte(data_buffer);
            }

            void RtmpContext::doHandleVideo(const DataBuffer::Ptr& data_buffer) {
                RTMPMediaSource::Ptr media_source(media_source_.lock());
                if (!media_source) {
                    LOG_DESTRUCT;
                    return;
                }

                media_source->OnHandleVideoData(data_buffer);
            }

        }
    }
}