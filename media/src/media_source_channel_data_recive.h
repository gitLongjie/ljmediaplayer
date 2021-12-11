#ifndef src_media_source_channel_data_recevie_h_
#define src_media_source_channel_data_recevie_h_

#include "src/lj_defined.h"

namespace LJMP {
    class MediaSourceChannelDataRecive {
        disable_copy(MediaSourceChannelDataRecive)

    public:
        virtual ~MediaSourceChannelDataRecive() = default;

        virtual void onUpdateScripte(const MediaConfig& config) = 0;

    protected:
        MediaSourceChannelDataRecive() = default;
    };
}

#endif // !src_media_source_channel_data_recevie_h_

