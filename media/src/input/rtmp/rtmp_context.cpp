#include "src/input/rtmp/rtmp_context.h"

#include "src/log.h"
#include "src/input/rtmp/rtmp_utils.h"

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

                if (!Rtmp::Utils::parseUrl(url_, &protocol_, &host_, &port_, &app_name_, &play_path_)) {
                    LOGE("{} parese failed", url_);
                    return false;
                }
                LOGI("protocol={}, host={}, port={}, app_name={}, play_path={}",
                    protocol_, host_, port_, app_name_, play_path_);

                return true;
            }

            void RtmpContext::uninitialzie() {

            }

        }
    }
}