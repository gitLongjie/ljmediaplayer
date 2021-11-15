#include "src/media.h"

#include <memory>
#include <cassert>
#include <chrono>

#include "src/utils.h"

#include "src/log.h"
#include "src/input/input_media_source_manager.h"
#include "src/network/network_manager_std.h"
#include "ljmedia/error_code.h"

namespace LJMP {
    Media* s_media = nullptr;

    Media::Media()
        : main_task_queue_("main")
        , callback_task_queue_("callback") 
        , io_task_queue_(std::make_shared<TaskQueue>("io")) {
        assert(nullptr == s_media);

        s_media = this;
    }

    Media::~Media() {
        main_task_queue_.stop();
        callback_task_queue_.stop();
        io_task_queue_->stop();
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
            return;
        }

        input_media_source_manager_ = Input::InputMediaSourceManager::create();
        if (!input_media_source_manager_->initialize()) {
            return ;
        }

    }

    void Media::doUninitialize(MediaWPtr wThis) {
        LOG_ENTER;

        input_media_source_manager_->uninitialize();
        network_manger_->uninitialize();
        input_media_source_manager_.reset();
        network_manger_.reset();

        spink_lock_.unlock();
    }

    void Media::doOpenUrl(const std::string url, MediaWPtr wThis) {
        LOG_ENTER;
        std::shared_ptr<Input::InputMediaSourceManager> media_source_manager =
            std::dynamic_pointer_cast<Input::InputMediaSourceManager>(input_media_source_manager_);
        if (!media_source_manager) {
            LOGE("media source manager is nullptr");
            errorCallbak(error_code_open_failed, "open failed");
            return;
          //  return media_source_manager->open(szUrl);
        }

        if (!media_source_manager->open(url)) {
            LOGE("open failed: {}", url);
            errorCallbak(error_code_open_failed, "open failed");
        }
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
        auto task = createTask(std::bind(&Media::doOpenUrl, this, szUrl, shared_from_this()));
        invoke(task);
        return true;
    }

    bool Media::initialize(errorCallback callback) {
        error_callback_ = callback;
        auto task = createTask(std::bind(&Media::doInitialize, this, shared_from_this()));
        invoke(task);
        return true;
    }

    void Media::uninitialize() {
        _run_do_unintialize = false;
        auto task = createTask(std::bind(&Media::doUninitialize, this, shared_from_this()));
        invoke(task);

        spink_lock_.lock();
    }

    void Media::invoke(const TaskPtr& task) {
        main_task_queue_.push(task);
    }

    void Media::invoke(const TaskPtr& task, uint16_t delay) {
        main_task_queue_.push(task, delay);
    }

    void Media::errorCallbak(int code, const char* msg) {
        auto task = createTask(std::bind(&Media::doErrorCallback, this, code, msg, shared_from_this()));
        invoke(task);
    }

} // namespace LJMP 
