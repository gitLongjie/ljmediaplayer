#ifndef src_input_rtmp_context_h
#define src_input_rtmp_context_h

#include <memory>
#include <string>

#include "src/lj_defined.h"

#include "src/network/channel.h"

namespace LJMP {
    namespace Input {
        namespace Rtmp {
            class RtmpContext {
                disable_copy(RtmpContext)

            public:
                static std::shared_ptr<RtmpContext> create(const std::string& url);

            public:
                virtual ~RtmpContext();

                bool intialize();
                void uninitialzie();

                bool connectServer();

            protected:
                explicit RtmpContext(const std::string& url);

            private:
                std::string url_;

                int protocol_ = 0;
                std::string host_;
                short port_ = 0;
                std::string app_name_;
                std::string play_path_;

                Network::ChannelPtr channel_;
            };

            using RtmpContextPtr = std::shared_ptr<RtmpContext>;
        }
    }
}

#endif // !src_input_rtmp_context_h
