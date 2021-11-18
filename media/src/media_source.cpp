#include "src/media_source.h"

#include "src/log.h"

namespace LJMP {

    MediaSource::MediaSource(const TaskQueuePtr task_queue) : task_queue_(task_queue) {}

    bool MediaSource::open(const std::string& url) {
        auto id = std::this_thread::get_id();
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
        auto id = std::this_thread::get_id();
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