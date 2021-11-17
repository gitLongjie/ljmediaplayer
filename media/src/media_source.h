#ifndef src_media_source_h_
#define src_media_source_h_

#include <memory>
#include <string>

#include "src/lj_defined.h"
#include "src/task_queue.h"

namespace LJMP {
    class MediaSource;
    using MediaSourcePtr = std::shared_ptr<MediaSource>;
    using MediaSourceWPtr = std::weak_ptr<MediaSource>;

    class MediaSource : public std::enable_shared_from_this<MediaSource> {
    public:
        virtual ~MediaSource() = default;

        bool open(const std::string& url);
        void close();

    protected:
        MediaSource(const TaskQueuePtr task_queue);

        void invoke(const TaskPtr& task);
        void invoke(const TaskPtr& task, uint16_t delay);

        virtual bool doOpen(const std::string& url) = 0;
        virtual void doClose() = 0;

    private:
        void openSource(std::string url, MediaSourceWPtr wThis);
        void closeSource(MediaSourceWPtr wThis);

    private:
        TaskQueuePtr task_queue_;
    };
} // namespace LJMP


#endif // ! src_media_source_h_
