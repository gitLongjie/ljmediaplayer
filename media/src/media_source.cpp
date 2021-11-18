#include "src/media_source.h"

#include "src/log.h"
#include "src/media.h"
#include "ljmedia/error_code.h"

namespace LJMP {

    MediaSource::MediaSource(const TaskQueuePtr task_queue) : task_queue_(task_queue) {}

    bool MediaSource::open(const std::string& url) {
        MediaSourceWPtr wThis(shared_from_this());
        auto task = createTask(std::bind(&MediaSource::openSource, this, url, wThis));
        invoke(task);
        return true;
    }

    void MediaSource::close() {
        MediaSourceWPtr wThis(shared_from_this());
        auto task = createTask(std::bind(&MediaSource::closeSource, this, wThis));
        invoke(task);
    }

    void MediaSource::invoke(const TaskPtr& task) {
        if (task_queue_) {
            task_queue_->push(task);
        }
    }

    void MediaSource::invoke(const TaskPtr& task, uint16_t delay) {
        if (task_queue_) {
            task_queue_->push(task, delay);
        }
    }

    void MediaSource::openSource(std::string url, MediaSourceWPtr wThis) {
        LOGI("open url {}", url);
        MediaSourcePtr self = wThis.lock();
        if (!self) {
            LOGE("this object is destruct {}", (long long)this);
            std::string msg("open url failed url=");
            msg.append(url);
            Media::getInstance()->errorCallbak(error_code_open_failed, msg);
            return;
        }

        if (!doOpen(url)) {
            doClose();
            return;
        }
    }

    void MediaSource::closeSource(MediaSourceWPtr wThis) {
        MediaSourcePtr self = wThis.lock();
        if (!self) {
            LOGE("this object is destruct {}", (long long)this);
            return;
        }

        doClose();
    }

}