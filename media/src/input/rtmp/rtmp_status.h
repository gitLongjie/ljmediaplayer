#ifndef src_input_rtmp_rtmp_status_h_
#define src_input_rtmp_rtmp_status_h_

#include <memory>

#include "src/lj_defined.h"

#include "src/input/rtmp/rtmp.h"

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

            protected:
                explicit RtmpStatus(const std::shared_ptr<RtmpContext>& rtmp_context);

                std::shared_ptr<RtmpContext> getRtmpContext() { return rtmp_context_.lock(); }
                
            private:
                std::weak_ptr<RtmpContext> rtmp_context_;
            };

            class RtmpConnectStatus : public RtmpStatus {
                disable_copy(RtmpConnectStatus);

            protected:
                ~RtmpConnectStatus() override;

            public:
                explicit RtmpConnectStatus(const std::shared_ptr<RtmpContext>& rtmp_context) noexcept;
            };

            class RtmpReaderStatus : public RtmpStatus {
                disable_copy(RtmpReaderStatus)

            public:
                using Ptr = std::shared_ptr<RtmpReaderStatus>;
                using WPtr = std::weak_ptr<RtmpReaderStatus>;

            public:
                ~RtmpReaderStatus() override = default;
                
                virtual void read(RTMP* rtmp) = 0;

            protected:
                explicit RtmpReaderStatus(const std::shared_ptr<RtmpContext>& rtmp_context);
            };


            class RtmpReadFLVHeaderStatus : public RtmpReaderStatus {
                disable_copy(RtmpReadFLVHeaderStatus)

            public:
                static RtmpReaderStatus::Ptr create(const std::shared_ptr<RtmpContext>& rtmp_context);

            public:
                ~RtmpReadFLVHeaderStatus() override = default;

                void read(RTMP* rtmp) override;

            protected:
                explicit RtmpReadFLVHeaderStatus(const std::shared_ptr<RtmpContext>& rtmp_context);

            };

            class RtmpReadFLVTagDataStatus : public RtmpReaderStatus {
                disable_copy(RtmpReadFLVTagDataStatus)

            public:
                static RtmpReaderStatus::Ptr create(const std::shared_ptr<RtmpContext>& rtmp_context);

            public:
                ~RtmpReadFLVTagDataStatus() override = default;

                void read(RTMP* rtmp) override;


            protected:
                explicit RtmpReadFLVTagDataStatus(const std::shared_ptr<RtmpContext>& rtmp_context);

            };

        }
    }
}

#endif // !src_input_rtmp_rtmp_status_h_
