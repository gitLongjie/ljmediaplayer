#ifndef src_input_rtmp_context_h
#define src_input_rtmp_context_h

#include <memory>
#include <string>

#include "src/lj_defined.h"

#include "src/network/channel.h"

namespace LJMP {
    namespace Input {
        namespace Rtmp {

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
                int getProtocol() const { return protocol_; }
                const std::string& getAppName() const { return app_name_; }

            protected:
                explicit RtmpContext(const std::string& url);

            private:
                std::string url_;

                int protocol_ = 0;
                std::string host_;
                short port_ = 0;
                std::string app_name_;
                std::string play_path_;

                Network::Channel::Ptr channel_;
                std::shared_ptr<RtmpStatus> rtmp_status_;

                int num_invokes_ = 0;
            };
        }
    }
}

#endif // !src_input_rtmp_context_h
