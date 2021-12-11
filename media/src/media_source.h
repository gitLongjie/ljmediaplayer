#ifndef src_media_source_h_
#define src_media_source_h_

#include <memory>
#include <string>

#include "src/lj_defined.h"
#include "src/task_queue_object.h"
#include "src/spin_lock.h"

namespace LJMP {
    class MediaChannel;

    class MediaSource : public TaskQueueObject {
    public:
        using Ptr = std::shared_ptr<MediaSource>;

    public:
        ~MediaSource() override;
        void setMediaChannel(std::weak_ptr<MediaChannel> media_channel) {
            media_channel_ = media_channel;
        }

        bool start();
        void stop();

        void updateMediaConfig();

    protected:
        explicit MediaSource(const std::string& url, const TaskQueue::Ptr& task_queue);

        virtual bool doOpen(const std::string& url) = 0;
        virtual void doClose() = 0;

        std::shared_ptr<MediaChannel> getMediaChannel() const; 

    private:
        void openSource(std::string url, WPtr wThis);
        void closeSource(WPtr wThis);

    private:
        SpinLock spin_lock_;

        std::weak_ptr<MediaChannel> media_channel_;
        const std::string url_;
    };
} // namespace LJMP


#endif // ! src_media_source_h_
