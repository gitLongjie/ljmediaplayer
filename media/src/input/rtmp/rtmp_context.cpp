#include "src/input/rtmp/rtmp_context.h"

#include "src/log.h"
#include "src/input/rtmp/rtmp_utils.h"
#include "src/network/socket.h"
#include "src/media.h"

namespace LJMP {
    namespace Input {
        namespace Rtmp {

            RtmpContextPtr RtmpContext::create(const std::string& url) {
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

                if (!Rtmp::RtmpUtils::parseUrl(url_, &protocol_, &host_, &port_, &app_name_, &play_path_)) {
                    LOGE("{} parese failed", url_);
                    return false;
                }
                LOGI("protocol={}, host={}, port={}, app_name={}, play_path={}",
                    protocol_, host_, port_, app_name_, play_path_);

                return true;
            }

            void RtmpContext::uninitialzie() {
                LOG_ENTER;

                if (channel_) {
                    channel_->disconnect();
                    channel_.reset();
                }
            }

            bool RtmpContext::connectServer() {
                LOG_ENTER;

                if (channel_) {
                    
                }

                Network::SocketPtr s = Network::Socket::create(Network::Socket::Model::TCP);
                if (!s->connect(host_, port_)) {
                    LOGE("connect server failed {}", host_);
                    return false;
                }
                s->enableNoDelay(true);
                s->enableTimeout(true, 30);
                channel_ = Network::Channel::create(Media::getInstance()->getIOTaskQueue(), s);
                return true;
            }

        }
    }
}