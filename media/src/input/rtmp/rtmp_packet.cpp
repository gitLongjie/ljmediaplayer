#include "src/input/rtmp/rtmp_packet.h"

namespace LJMP {
    namespace Input {
        namespace Rtmp {

            RtmpPacket::Ptr RtmpPacket::create() {
                struct Creator : public RtmpPacket {
                    Creator() : RtmpPacket() {}
                    ~Creator() override = default;
                };

                return std::make_shared<Creator>();
            }

            RtmpPacket::RtmpPacket() {
                reset();
            }

            RtmpPacket::~RtmpPacket() {
                reset();
                if (chunk_) {
                    delete chunk_;
                }
            }

            bool RtmpPacket::alloca(unsigned int size) {
                if (size > SIZE_MAX - RTMP_MAX_HEADER_SIZE) {
                    return false;
                }

                body_ = DataBuffer::create(size + RTMP_MAX_HEADER_SIZE);
                if (!body_) {
                    return false;
                }

                body_->setOffset(RTMP_MAX_HEADER_SIZE);
                body_size_ = 0;
                return true;
            }

            void RtmpPacket::free() {
                if (body_) {
                    body_.reset();
                }

                body_ = nullptr;
                body_size_ = 0;
            }

            void RtmpPacket::reset() {
                header_type_ = 0;
                packet_type_ = 0;
                has_abs_time_ = false;
                channel_ = 0;
                time_stamp_ = 0;
                info_filed_ = 0;
                body_size_ = 0;
                bytes_read_ = 0;

                free();
            }

        }
    }
}