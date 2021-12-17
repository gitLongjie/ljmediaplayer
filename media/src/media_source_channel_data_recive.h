#ifndef src_media_source_channel_data_recevie_h_
#define src_media_source_channel_data_recevie_h_

#include "src/lj_defined.h"

namespace LJMP {
    class DataBuffer;

    class MediaSourceChannelDataRecive {
        disable_copy(MediaSourceChannelDataRecive)

    public:
        virtual ~MediaSourceChannelDataRecive() = default;

        virtual void onReciveScripte(const std::shared_ptr<MediaConfig>& config) = 0;
        virtual void onReciveVideo(const std::shared_ptr<DataBuffer>& data_buffer) = 0;

    protected:
        MediaSourceChannelDataRecive() = default;
    };
}

#endif // !src_media_source_channel_data_recevie_h_

