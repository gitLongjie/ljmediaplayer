#ifndef src_media_source_h_
#define src_media_source_h_

#include <memory>
#include <string>

#include "src/lj_defined.h"
#include "src/task_queue_object.h"
#include "src/spin_lock.h"

#include "src/media_context.h"

namespace LJMP {
    class MediaSource : public TaskQueueObject {
    public:
        using Ptr = std::shared_ptr<MediaSource>;

    public:
        ~MediaSource() override;
        void setMediaContext(MediaContext::WPtr media_contxt) {
            media_context_ = media_contxt;
        }

        bool open(const std::string& url);
        void close();

        void updateMediaConfig();

    protected:
        explicit MediaSource(const TaskQueue::Ptr task_queue);

        virtual bool doOpen(const std::string& url) = 0;
        virtual void doClose() = 0;

        MediaContext::Ptr getMediaSource() const {
            return std::dynamic_pointer_cast<MediaContext>(media_context_.lock());
        }

    private:
        void openSource(std::string url, WPtr wThis);
        void closeSource(WPtr wThis);

    private:
        SpinLock spin_lock_;

        MediaContext::WPtr media_context_;
    };
} // namespace LJMP


#endif // ! src_media_source_h_
