#ifndef src_media_h_
#define src_media_h_

#include <memory>
#include <string>

#include "src/lj_defined.h"

#include "src/task_queue.h"
#include "ljmedia/lj_media.h"

namespace LJMP {
    class MediaSourceManager;
    class NetworkManager;

    class Media;
    using MediaPtr = std::shared_ptr<Media>;
    using MediaWPtr = std::weak_ptr<Media>;

    class Media : public std::enable_shared_from_this<Media> {
        disable_copy(Media)

    public:
        Media();
        ~Media();
        
        bool initialize(errorCallback callback);
        void uninitialize();

        void invoke(const TaskPtr& task);
        void invoke(const TaskPtr& task, uint16_t delay);

        void errorCallbak(int code, const char* msg);
        
    public:
        bool openUrl(const char* szUrl);

    public:
        static Media* getInstance();

    protected:
        void doInitialize(MediaWPtr wThis);
        void doUninitialize(MediaWPtr wThis);

        void doOpenUrl(const std::string url, MediaWPtr wThis);
        void doErrorCallback(int code, const std::string msg, MediaWPtr wThis);
        
    private:
        std::shared_ptr<MediaSourceManager> input_media_source_manager_;
        std::shared_ptr<NetworkManager> network_manger_;

    private:
        TaskQueue main_task_queue_;
        TaskQueue callback_task_queue_;
        std::shared_ptr<TaskQueue> io_task_queue_;

        errorCallback error_callback_ = nullptr;
        bool _run_do_unintialize = true;
    };
}

#endif  // src_media_h_
