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
            class RtmpPacket;

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

                bool sendPacket(const std::shared_ptr<RtmpPacket>& packet);

                int& numInvokes() { return num_invokes_; }
                const std::shared_ptr<RtmpLink>& getLink() const { return rtmp_link_; }
                double getAudioCodecs() const { return audioCodecs_; }
                double getVideoCodecs() const { return videoCodecs_; }
                double getEncoding() const { return encoding_; }
                unsigned char getSendEncoding() const { return send_encoding_; }

            protected:
                explicit RtmpContext(const std::string& url);

                void reset();

            private:
                std::string url_;

                Network::Channel::Ptr channel_;
                std::shared_ptr<RtmpStatus> rtmp_status_;
                std::shared_ptr<RtmpLink> rtmp_link_;

                int num_invokes_ = 0;
                unsigned char send_encoding_ = 0;

                double audioCodecs_ = 1319.0;
                double videoCodecs_ = 252.0;
                double encoding_ = 0.0;
            };
        }
    }
}

#endif // !src_input_rtmp_context_h
