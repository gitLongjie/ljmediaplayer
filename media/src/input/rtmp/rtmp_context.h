#ifndef src_input_rtmp_context_h
#define src_input_rtmp_context_h

#include <memory>
#include <string>

#include "src/lj_defined.h"

#include "src/network/channel.h"

namespace LJMP {
    namespace Input {
        namespace Rtmp {

            class RtmpLink;
            class RtmpStatus;

            class RtmpContext : public std::enable_shared_from_this<RtmpContext> {
                disable_copy(RtmpContext)

            public:
                using Ptr = std::shared_ptr<RtmpContext>;
                using WPtr = std::weak_ptr<RtmpContext>;

                static std::shared_ptr<RtmpContext> create(const std::string& url);

            public:
                virtual ~RtmpContext();

                bool intialize();
                void uninitialzie();

                bool connectServer();

                void switchStatus(std::shared_ptr<RtmpStatus> status);

                int& numInvokes() { return num_invokes_; }
                int getProtocol() const;
                std::string getAppName() const;

            protected:
                explicit RtmpContext(const std::string& url);

            private:
                std::string url_;

                Network::Channel::Ptr channel_;
                std::shared_ptr<RtmpStatus> rtmp_status_;
                std::shared_ptr<RtmpLink> rtmp_link_;

                int num_invokes_ = 0;
            };
        }
    }
}

#endif // !src_input_rtmp_context_h
