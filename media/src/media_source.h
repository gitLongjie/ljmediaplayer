#ifndef src_media_source_h_
#define src_media_source_h_

#include <memory>
#include <string>

#include "src/lj_defined.h"
#include "src/task_queue.h"
#include "src/spin_lock.h"

namespace LJMP {

    class MediaSource : public std::enable_shared_from_this<MediaSource> {
    public:
        using Ptr = std::shared_ptr<MediaSource>;
        using WPtr = std::weak_ptr<MediaSource>;
    public:
        virtual ~MediaSource() = default;

        bool open(const std::string& url);
        void close();

        void invoke(const TaskPtr& task);
        void invoke(const TaskPtr& task, uint16_t delay);

    protected:
        MediaSource(const TaskQueuePtr task_queue);

        virtual bool doOpen(const std::string& url) = 0;
        virtual void doClose() = 0;

    private:
        void openSource(std::string url, WPtr wThis);
        void closeSource(WPtr wThis);

    private:
        TaskQueuePtr task_queue_;
        SpinLock spin_lock_;
    };
} // namespace LJMP


#endif // ! src_media_source_h_
