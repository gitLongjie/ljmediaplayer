#include "src/input/rtmp/rtmp_context.h"

#include "src/log.h"
#include "src/network/socket.h"
#include "src/media.h"

#include "src/utils.h"
#include "src/input/rtmp/rtmp_link.h"
#include "src/input/rtmp/rtmp_status.h"
#include "src/input/rtmp/rtmp_utils.h"

namespace LJMP {
    namespace Input {
        namespace Rtmp {

            RtmpContext::Ptr RtmpContext::create(const std::string& url) {
                struct Createor : public RtmpContext {
                    explicit Createor(const std::string& url) : RtmpContext(url) {}
                    ~Createor() override = default;
                };
                return std::make_shared<Createor>(url);
            }

            RtmpContext::RtmpContext(const std::string& url)
                : url_(url){
                LOGI("actor {} url={}", (long long)this, url);

            }
            
            RtmpContext::~RtmpContext() {
                LOGI("dctor {}", (long long)this);
            }

            bool RtmpContext::intialize() {
                LOG_ENTER;

                reset();

                if (rtmp_link_) {
                    rtmp_link_.reset();
                }
                rtmp_link_ = RtmpLink::create();
                if (!rtmp_link_->parseUrl(url_)) {
                    LOGE("{} parese failed", url_);
                    return false;
                }

                const RtmpLink::Link& link = rtmp_link_->getLink();
                LOGI("protocol={}, host={}, port={}, app_name={}, play_path={}",
                    link.protocol_, RtmpUtils::avalToString(link.host_name_), link.port_,
                    RtmpUtils::avalToString(link.app_), RtmpUtils::avalToString(link.playpath_));

                return true;
            }

            void RtmpContext::uninitialzie() {
                LOG_ENTER;

                if (channel_) {
                    channel_->disconnect();
                    channel_.reset();
                }

                if (rtmp_link_) {
                    rtmp_link_.reset();
                }
            }

            bool RtmpContext::connectServer() {
                LOG_ENTER;

                const RtmpLink::Link& link = rtmp_link_->getLink();
                Network::SocketPtr s = Network::Socket::create(Network::Socket::Model::TCP);
                const std::string ip = RtmpUtils::avalToString(link.host_name_);
                if (!s->connect(ip, link.port_)) {
                    LOGE("connect server failed {}", ip);
                    return false;
                }
                s->enableNoDelay(true);
                s->enableTimeout(true, 30);
                channel_ = Network::Channel::create(Media::getInstance()->getIOTaskQueue(), s);

                WPtr wThis(shared_from_this());
                rtmp_status_ = RtmpHandShakeStatus::create(wThis, channel_);
                return true;
            }

            void RtmpContext::switchStatus(std::shared_ptr<RtmpStatus> status) {
                LOG_ENTER;

                if (status != rtmp_status_) {
                    if (status) {
                        LOGI("switch to status: {}", status->getName());
                    }
                    rtmp_status_ = status;
                    rtmp_status_->switchToSelf();
                }
            }

            bool RtmpContext::sendPacket(const std::shared_ptr<RtmpPacket>& packet) {
                return false;
            }

            void RtmpContext::reset() {
                num_invokes_ = 0;
                audioCodecs_ = 1319.0;
                videoCodecs_ = 252.0;
            }
        }
    }
}