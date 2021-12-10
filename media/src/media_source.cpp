#include "src/media_source.h"

#include "src/log.h"
#include "src/media.h"
#include "ljmedia/error_code.h"

namespace LJMP {

    MediaSource::MediaSource(const TaskQueue::Ptr& task_queue) : TaskQueueObject(task_queue) {
        LOG_CREATER;
    }

    MediaSource::~MediaSource() {
        LOG_DESTRUCT;
    }

    bool MediaSource::open(const std::string& url) {
        WPtr wThis(shared_from_this());
        auto task = createTask(std::bind(&MediaSource::openSource, this, url, wThis));
        invoke(task);
        return true;
    }

    void MediaSource::close() {
        WPtr wThis(shared_from_this());
        auto task = createTask(std::bind(&MediaSource::closeSource, this, wThis));
        invoke(task);

        spin_lock_.lock();
    }

    void MediaSource::openSource(std::string url, WPtr wThis) {
        LOGI("open url {}", url);
        TaskQueueObject::Ptr self(wThis.lock());
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

    void MediaSource::closeSource(WPtr wThis) {
        TaskQueueObject::Ptr self(wThis.lock());
        if (!self) {
            LOGE("this object is destruct {}", (long long)this);
            return;
        }

        doClose();
        spin_lock_.unlock();
    }

}