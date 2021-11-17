#include "src/input/rtmp/rtmp_utils.h"

#include "src/log.h"
#include "src/network/network_utils.h"

namespace LJMP {
    namespace Input {
        namespace Rtmp {

            bool RtmpUtils::parseUrl(const std::string& url, int* protocol, std::string* host, short* port,
                std::string* app_name, std::string* play_path) {
                if (nullptr == protocol || nullptr == host || nullptr == port) {
                    LOGE("param is nullptr, protocl={}, host={}, port={}",
                        (long long)protocol, (long long)host, (long long)port);
                    return false;
                }
                *protocol = RTMP_PROTOCOL_RTMP;

                size_t pos = url.find("://");
                if (std::string::npos == pos) {
                    LOGE("{} is not rmtp protocol", url);
                    return false;
                }
                {
                    std::string str_protocol = url.substr(0, pos);
                    if (str_protocol == kRtmp) {
                        *protocol = RTMP_PROTOCOL_RTMP;
                    }
                    else if (str_protocol == kRtmpt) {
                        *protocol = RTMP_PROTOCOL_RTMPT;
                    }
                    else if (str_protocol == kRtmps) {
                        *protocol = RTMP_PROTOCOL_RTMPS;
                    }
                    else if (str_protocol == kRtmpe) {
                        *protocol = RTMP_PROTOCOL_RTMPE;
                    }
                    else if (str_protocol == kRtmfp) {
                        *protocol = RTMP_PROTOCOL_RTMFP;
                    }
                    else if (str_protocol == kRtmpte) {
                        *protocol = RTMP_PROTOCOL_RTMPTE;
                    }
                    else if (str_protocol == kRtmpts) {
                        *protocol = RTMP_PROTOCOL_RTMPTS;
                    }
                } 
                
                std::string host_port_param = url.substr(pos + 3);
                pos = host_port_param.find("/");
                if (std::string::npos == pos) {
                    LOGE("{} is valid", url);
                    return false;
                }

                std::string host_post = host_port_param.substr(0, pos);
                LJMP::Network::NetworkUtils::splitHostPort(host_post, host, port);

                size_t port_pos = pos;
                pos = host_port_param.rfind("/");
                if (pos < port_pos) {
                    pos = port_pos + 1;
                }
                *app_name = host_port_param.substr(port_pos + 1, pos - port_pos - 1);

                *play_path = host_port_param.substr(pos + 1);
                
                return true;
            }

        }
    }
}