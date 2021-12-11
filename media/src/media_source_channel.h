#ifndef src_media_source_channel_h_
#define src_media_source_channel_h_

#include <memory>

#include "src/media_channel.h"

namespace LJMP {
    class MediaSource;
    class MediaSourceChannelDataRecive;

    class MediaSourceChannel : public MediaChannel {
        disable_copy(MediaSourceChannel)

    public:
        using Ptr = std::shared_ptr<MediaSourceChannel>;
        static Ptr create(const std::shared_ptr<MediaSource>& media_source, const TaskQueue::Ptr& task_queue);

    public:
        ~MediaSourceChannel() override;

        void registDataRecive(const std::shared_ptr<MediaSourceChannelDataRecive>& data_recive);
        void unregistDataRecive(const std::shared_ptr<MediaSourceChannelDataRecive>& data_recive);

        void reciveData(DataType data_type, void* data, WPtr wThis);

    protected:
        MediaSourceChannel(const std::shared_ptr<MediaSource>& media_source,
            const TaskQueue::Ptr& task_queue);

        void onRegistDataRecive(const std::shared_ptr<MediaSourceChannelDataRecive> data_recive, WPtr wThis);
        void onUnregistDataRecive(const std::shared_ptr<MediaSourceChannelDataRecive> data_recive, WPtr wThis);

        void onUpdateScript(const std::shared_ptr<MediaConfig>& config, WPtr wThis);

    private:
        std::shared_ptr<MediaSource> media_source_;

        using ListMediaSourceDataRecives = std::vector<std::shared_ptr<MediaSourceChannelDataRecive> >;
        ListMediaSourceDataRecives list_data_recives_;
    };
}

#endif // !src_media_source_channel_h_

