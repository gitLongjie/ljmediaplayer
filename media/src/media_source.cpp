#include "src/media_source.h"

#include "src/log.h"

namespace LJMP {

    MediaSource::MediaSource(const TaskQueuePtr task_queue) : task_queue_(task_queue) {}

    bool MediaSource::open(const std::string& url) {
        auto task = createTask(std::bind(&MediaSource::openSource, this, url, shared_from_this()));
        invoke(task);
        return true;
    }

    void MediaSource::close() {
        auto task = createTask(std::bind(&MediaSource::closeSource, this, shared_from_this()));
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