#ifndef src_media_source_h_
#define src_media_source_h_

#include <memory>
#include <string>

#include "src/lj_defined.h"
#include "src/task_queue_object.h"
#include "src/spin_lock.h"

namespace LJMP {
    class MediaSourceChannel;

    class MediaSource : public TaskQueueObject {
    public:
        using Ptr = std::shared_ptr<MediaSource>;
        using callbackFunc = std::function<void(DataType type, void* data)>;

    public:
        ~MediaSource() override;

        bool start();
        void stop();

        void setCallbackFunc(callbackFunc callback);

        void updateMediaConfig();

    protected:
        explicit MediaSource(const std::string& url, const TaskQueue::Ptr& task_queue);

        virtual bool doOpen(const std::string& url) = 0;
        virtual void doClose() = 0;


        callbackFunc getCallbackFunc() const {
            return callback_func_;
        }

    private:
        void openSource(std::string url, WPtr wThis);
        void closeSource(WPtr wThis);
        void onSetCallbackFunc(callbackFunc callback, WPtr wThis);

    private:
        SpinLock spin_lock_;
        const std::string url_;

        callbackFunc callback_func_ = nullptr;
    };
} // namespace LJMP


#endif // ! src_media_source_h_
