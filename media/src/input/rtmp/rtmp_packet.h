#ifndef src_input_rtmp_rtmp_packet_h_
#define src_input_rtmp_rtmp_packet_h_

#include <memory>

#include "src/lj_defined.h"
#include "src/input/rtmp/rtmp_define.h"
#include "src/data_buffer.h"

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
                using Ptr = std::shared_ptr<RtmpPacket>;
                using WPtr = std::weak_ptr<RtmpPacket>;

                static Ptr create();

            public:
                virtual ~RtmpPacket();

                bool alloca(unsigned int size);
                void free();
                void reset();

            public:
                void setHeaderType(unsigned char head_type) { header_type_ = head_type; }
                unsigned char getHeaderType() const { return header_type_; }

                void setPacketType(unsigned char packet_type) { packet_type_ = packet_type; }
                unsigned char getPacketType() const { return packet_type_; }

                void setHasAbsTime(unsigned char has) { has_abs_time_ = has; }
                unsigned char getHasAbsType() const { return has_abs_time_; }

                void setChannel(int channel) { channel_ = channel; }
                int getChannel() const { return channel_; }

                void setTimeStamp(unsigned int time_stamp) { time_stamp_ = time_stamp; }
                unsigned int getTimeStamp() const { return time_stamp_; }

                void setInfoFiled(unsigned int info_filed) { info_filed_ = info_filed; }
                unsigned int getInfoField() const { return info_filed_; }

                void setBody(const DataBuffer::Ptr& body) { body_ = body; }
                DataBuffer::Ptr getBody() const { return body_; }
                void setBodySize(unsigned int size) { body_size_ = size; }

            protected:
                RtmpPacket();

            private:
                unsigned char header_type_ = 0;
                unsigned char packet_type_;
                unsigned char has_abs_time_;
                int channel_;
                unsigned int time_stamp_;
                unsigned int info_filed_; /* last 4 bytes in a long header */
                unsigned int body_size_;
                unsigned int bytes_read_;
                RtmpChunk* chunk_;
                DataBuffer::Ptr body_;
            };
        }
    }
}

#endif // !src_input_rtmp_rtmp_packet_h_
