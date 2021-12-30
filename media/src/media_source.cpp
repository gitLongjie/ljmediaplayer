#include "src/media_source.h"

#include "src/log.h"
#include "src/media.h"
#include "ljmedia/error_code.h"
#include "src/media_channel.h"

namespace LJMP {

    MediaSource::MediaSource(const std::string& url, const TaskQueue::Ptr& task_queue)
        : TaskObject(task_queue, false), url_(url){
        LOG_CREATER;
    }

    MediaSource::~MediaSource() {
        LOG_DESTRUCT;
    }

    bool MediaSource::start() {
        if (url_.empty()) {
            LOGE("url is empty");
            return false;
        }
        WPtr wThis(shared_from_this());
        auto task = createTask(std::bind(&MediaSource::openSource, this, url_, wThis));
        invoke(task);
        return true;
    }

    void MediaSource::stop() {
        WPtr wThis(shared_from_this());
        auto task = createTask(std::bind(&MediaSource::closeSource, this, wThis));
        invoke(task);

        spin_lock_.lock();
    }

    void MediaSource::setCallbackFunc(callbackFunc callback) {
        LOG_ENTER;

        WPtr wThis(shared_from_this());
        if (isCurrentThread()) {
            onSetCallbackFunc(callback, wThis);
            return;
        }

        auto task = createTask(std::bind(&MediaSource::onSetCallbackFunc, this, callback, wThis));
        invoke(task);
    }

    void MediaSource::openSource(std::string url, WPtr wThis) {
        LOGI("open url {}", url);
        ObjectPtr::Ptr self(wThis.lock());
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
        ObjectPtr::Ptr self(wThis.lock());
        if (!self) {
            LOGE("this object is destruct {}", (long long)this);
            return;
        }

        doClose();
        spin_lock_.unlock();
    }

    void MediaSource::onSetCallbackFunc(callbackFunc callback, WPtr wThis) {
        LOG_ENTER;

        ObjectPtr::Ptr self(wThis.lock());
        if (!self) {
            LOGE("this object is destruct");
            return;
        }

        callback_func_ = callback;
    }

}