#ifndef src_input_rtmp_rtmp_status_h_
#define src_input_rtmp_rtmp_status_h_

#include <memory>

#include "src/lj_defined.h"

#include "src/network/socket.h"

namespace LJMP {
    namespace Input {
        namespace Rtmp {
            class RtmpStatus : public std::enable_shared_from_this<RtmpStatus> {
                disable_copy(RtmpStatus)

            public:
                using RtmpStatusPtr = std::shared_ptr<RtmpStatus>;

            public:
                virtual ~RtmpStatus() = default;

                virtual bool read(const Network::SocketPtr& sc) = 0;
                virtual int write(const Network::SocketPtr& sc) = 0;

                virtual RtmpStatusPtr getNext() = 0;

            protected:
                RtmpStatus() = default;
            };


            class RtmpHandShakeStatus : public RtmpStatus {
                disable_copy(RtmpHandShakeStatus)

            public:
                enum { RTMP_SIG_SIZE = 1536 };
                static RtmpStatus::RtmpStatusPtr create();

            public:
                ~RtmpHandShakeStatus() override = default;

                bool read(const Network::SocketPtr & sc) override;
                int write(const Network::SocketPtr& sc) override;
                RtmpStatusPtr getNext() override;

            protected:
                RtmpHandShakeStatus() = default;

                int handleShake1Write(const Network::SocketPtr& sc);
                int handleShake2Write(const Network::SocketPtr& sc);
                bool handleShake1(const Network::SocketPtr& sc);
                bool handleShake2(const Network::SocketPtr& sc);

            private:
                int hand_shake_count_ = 0;
                char clientbuf_[RTMP_SIG_SIZE + 1] = { 0 };
                char serversig_[RTMP_SIG_SIZE] = { 0 };
            };
        }
    }
}

#endif // !src_input_rtmp_rtmp_status_h_
