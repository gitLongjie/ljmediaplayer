#include "src/media.h"

#include <memory>
#include <cassert>
#include <chrono>

#include "ljmedia/error_code.h"

#include "src/utils.h"

#include "src/thread_pools.h"
#include "src/log.h"
#include "src/input/input_media_source_manager.h"
#include "src/network/network_manager_std.h"
#include "src/media_context.h"
#include "src/media_context_manager.h"
#include "src/media_source_channel.h"

namespace LJMP {
    Media* s_media = nullptr;

    Media::Media()
        : thread_pool_(ThreadPool::create(std::thread::hardware_concurrency()))
        , main_task_queue_("main")
        , callback_task_queue_("callback") 
        , media_task_queue_(TaskQueue::create("media"))
        , io_task_queue_(TaskQueue::create("io")) {
        assert(nullptr == s_media);

        s_media = this;
    }

    Media::~Media() {
        main_task_queue_.stop();
        callback_task_queue_.stop();
        media_task_queue_->stop();
        io_task_queue_->stop();
        thread_pool_->stop();
        s_media = nullptr;
    }

    Media* Media::getInstance() {
        return s_media;
    }

    void Media::doInitialize(MediaWPtr wThis) {
        LOG_ENTER;
        MediaPtr self = wThis.lock();
        if (!self) {
            LOGE("object is destruct");
            return;
        }

        network_manger_ = Network::NetworkManagerStd::create(io_task_queue_);
        if (!network_manger_->initialize()) {
            LOGE("net work manager initialize failed");
            return;
        }

        input_media_source_manager_ = Input::InputMediaSourceManager::create();
        if (!input_media_source_manager_->initialize()) {
            LOGE("media source manager initialize failed");
            return ;
        }

        media_context_manger_ = MediaContextManager::create();
    }

    void Media::doUninitialize(MediaWPtr wThis) {
        LOG_ENTER;
        media_context_manger_.reset();

        input_media_source_manager_->uninitialize();
        network_manger_->uninitialize();
        input_media_source_manager_.reset();
        network_manger_.reset();

        spink_lock_.unlock();
    }

    void Media::doOpenUrl(const std::string url, MediaWPtr wThis) {
        LOG_ENTER;
        if (media_context_manger_->isContain(url)) {
            errorCallbak(error_code_opened, "open failed");
            return;
        }

        MediaSource::Ptr media_source = input_media_source_manager_->getMediaSource(url);
        if (nullptr == media_source) {
            LOGE("get media source failed : {}", url);
            errorCallbak(error_code_open_failed, "open failed");
            return;
        }

        MediaChannel::Ptr media_source_channel = MediaSourceChannel::create(media_source, media_task_queue_);
        //MediaChannel::Ptr;
        MediaContext::Ptr media_context = MediaContext::create(media_task_queue_);
        media_context_manger_->addMediaContext(media_context, url);

        media_context->updateMediaChannel(media_source_channel);
       // return false;
    }

    void Media::doErrorCallback(int code, const std::string msg, MediaWPtr wThis) {
        MediaPtr self = wThis.lock();
        if (!self) {
            LOGE("this is destruct");
            return;
        }

        if (nullptr == error_callback_) {
            LOGE("error callback is nullptr");
            return;
        }

        error_callback_(code, msg.c_str());
    }

    bool Media::openUrl(const char* szUrl) {
        MediaWPtr wThis(shared_from_this());
        auto task = createTask(std::bind(&Media::doOpenUrl, this, szUrl, wThis));
        invoke(task);
        return true;
    }

    bool Media::initialize(errorCallback callback) {
        MediaWPtr wThis(shared_from_this());

        error_callback_ = callback;
        auto task = createTask(std::bind(&Media::doInitialize, this, wThis));
        invoke(task);
        return true;
    }

    void Media::uninitialize() {
        MediaWPtr wThis(shared_from_this());
        _run_do_unintialize = false;
        auto task = createTask(std::bind(&Media::doUninitialize, this, wThis));
        invoke(task);

        spink_lock_.lock();
    }

    void Media::invoke(const Task::Ptr& task) {
        main_task_queue_.push(task);
    }

    void Media::invoke(const Task::Ptr& task, uint16_t delay) {
        main_task_queue_.push(task, delay);
    }

    void Media::errorCallbak(int code, const std::string& msg) {
        MediaWPtr wThis(shared_from_this());
        std::string message(msg);
        auto task = createTask(std::bind(&Media::doErrorCallback, this, code, msg, wThis));
        invoke(task);
    }

} // namespace LJMP 
