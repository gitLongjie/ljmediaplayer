#ifndef src_input_rtmp_rtmp_packet_h_
#define src_input_rtmp_rtmp_packet_h_

#include <memory>

#include "src/lj_defined.h"
#include "src/input/rtmp/rtmp_define.h"

namespace LJMP {
    namespace Input {
        namespace Rtmp {
            struct RtmpChunk {
                int header_size_;
                int chunk_size_;
                char* chunk_;
                char header_[RTMP_MAX_HEADER_SIZE];
            };

            class RtmpPacket : public std::enable_shared_from_this<RtmpPacket> {
                disable_copy(RtmpPacket)

            public:
                using RtmpPacketPtr = std::shared_ptr<RtmpPacket>;
                using RtmpPacketWPtr = std::weak_ptr<RtmpPacket>;

                static RtmpPacketPtr create();

            public:
                virtual ~RtmpPacket();

            protected:
                RtmpPacket();

            private:
                unsigned char header_type_;
                unsigned char packet_type_;
                unsigned char has_abs_time_;
                int channel_;
                unsigned int time_stamp_;
                unsigned int info_filed_; /* last 4 bytes in a long header */
                unsigned int body_size_;
                unsigned int bytes_read_;
                RtmpChunk* chunk_;
                char* body_;
            };
        }
    }
}

#endif // !src_input_rtmp_rtmp_packet_h_
