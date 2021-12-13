#ifndef src_media_h_
#define src_media_h_

#include <memory>
#include <string>

#include "src/lj_defined.h"

#include "src/task_queue.h"
#include "ljmedia/lj_media.h"

#include "src/spin_lock.h"

namespace LJMP {
    class MediaSourceManager;
    class NetworkManager;
    class ThreadPool;
    class MediaContextManager;
    class MediaCodecManager;

    class Media;
    using MediaPtr = std::shared_ptr<Media>;
    using MediaWPtr = std::weak_ptr<Media>;

    class Media : public std::enable_shared_from_this<Media> {
        disable_copy(Media)

    public:
        static Media* getInstance();

    public:
        Media();
        ~Media();
        
        bool initialize(errorCallback callback);
        void uninitialize();

        void invoke(const Task::Ptr& task);
        void invoke(const Task::Ptr& task, uint16_t delay);

        void errorCallbak(int code, const std::string& msg);
        
    public:
        bool openUrl(const char* szUrl);

        TaskQueue::Ptr getIOTaskQueue() const { return io_task_queue_; }
        std::shared_ptr<NetworkManager> getNetworkManager() const { return network_manger_; }
        std::shared_ptr<ThreadPool> getThreadPool() const { return thread_pool_; }
        std::shared_ptr<MediaCodecManager> getMediaCodecManager() const { return media_codec_manager_; }

    protected:
        void doInitialize(MediaWPtr wThis);
        void doUninitialize(MediaWPtr wThis);

        void doOpenUrl(const std::string url, MediaWPtr wThis);
        void doErrorCallback(int code, const std::string msg, MediaWPtr wThis);
        
    private:
        std::shared_ptr<MediaSourceManager> input_media_source_manager_;
        std::shared_ptr<NetworkManager> network_manger_;
        std::shared_ptr<MediaContextManager> media_context_manger_;
        std::shared_ptr<MediaCodecManager> media_codec_manager_;

    private:
        std::shared_ptr<ThreadPool> thread_pool_;
        TaskQueue main_task_queue_;
        TaskQueue callback_task_queue_;
        TaskQueue::Ptr media_task_queue_;
        std::shared_ptr<TaskQueue> io_task_queue_;

        errorCallback error_callback_ = nullptr;
        bool _run_do_unintialize = true;

        SpinLock spink_lock_;
    };
}

#endif  // src_media_h_
