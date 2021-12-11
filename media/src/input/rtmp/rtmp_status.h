#ifndef src_input_rtmp_rtmp_status_h_
#define src_input_rtmp_rtmp_status_h_

#include <memory>

#include "src/lj_defined.h"

#include "src/data_buffer.h"
#include "src/network/channel.h"

namespace LJMP {
    namespace Input {
        namespace Rtmp {

            extern const char kRtmpStatusUnknown[];
            extern const char kRtmpStatusHandShake[];
            extern const char kRtmpStatusConnecting[];

            class RtmpContext;

            class RtmpStatus : public std::enable_shared_from_this<RtmpStatus> {
                disable_copy(RtmpStatus)

            public:
                using Ptr = std::shared_ptr<RtmpStatus>;
                using RtmpStatusWPtr = std::weak_ptr<RtmpStatus>;

            public:
                virtual ~RtmpStatus();

                virtual bool read() = 0;
                virtual int write() = 0;

                virtual const char* getName() const { return kRtmpStatusUnknown; }
                void readCallback(const Network::Channel::Ptr& channel, RtmpStatusWPtr wThis);

                virtual void switchToSelf();

            protected:
                explicit RtmpStatus(std::weak_ptr<RtmpContext> rtmp_context, Network::Channel::Ptr channel);

                bool writeInChannel(const DataBuffer::Ptr& buffer);
                Network::Channel::Ptr getChannel() const { return channel_.lock(); }

                std::shared_ptr<RtmpContext> getRtmpContext() { return rtmp_context_.lock(); }
                
                void writeStatusCallback(bool status, RtmpStatusWPtr wThis);


            private:
                std::weak_ptr<RtmpContext> rtmp_context_;
                Network::Channel::WPtr channel_;
                bool write_failed_ = false;
            };


            class RtmpHandShakeStatus : public RtmpStatus {
                disable_copy(RtmpHandShakeStatus)

            public:
                enum { RTMP_SIG_SIZE = 1536 };
                static RtmpStatus::Ptr create(std::weak_ptr<RtmpContext> rtmp_context,
                    Network::Channel::Ptr channel);

            public:
                ~RtmpHandShakeStatus() override = default;

                bool read() override;
                int write() override;
                const char* getName() const override {
                    return kRtmpStatusHandShake;
                }

            protected:
                RtmpHandShakeStatus(std::weak_ptr<RtmpContext> rtmp_context, Network::Channel::Ptr channel);

                int handleShake1Write();
                int handleShake2Write();
                bool handleShake1();
                bool handleShake2();

            private:
                int hand_shake_count_ = 0;
                char clientbuf_[RTMP_SIG_SIZE + 1] = { 0 };
                char serversig_[RTMP_SIG_SIZE] = { 0 };
            };

            class RtmpConnectStatus : public RtmpStatus {
                disable_copy(RtmpConnectStatus)

            public:
                static RtmpStatus::Ptr create(std::weak_ptr<RtmpContext> rtmp_context, Network::Channel::Ptr channel);

            public:
                ~RtmpConnectStatus() override;

                bool read() override;
                int write() override;
                
            protected:
                RtmpConnectStatus(std::weak_ptr<RtmpContext> rtmp_context, Network::Channel::Ptr channel);


            };
        }
    }
}

#endif // !src_input_rtmp_rtmp_status_h_
