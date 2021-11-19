#include "src/input/rtmp/rtmp_status.h"

#include "src/log.h"
#include "src/utils.h"

namespace LJMP {
    namespace Input {
        namespace Rtmp {

            RtmpStatus::RtmpStatusPtr RtmpHandShakeStatus::create() {
                struct Creator : public RtmpHandShakeStatus {
                    Creator() : RtmpHandShakeStatus() {}
                    ~Creator() override = default;
                };
                return std::make_shared<Creator>();
            }

            bool RtmpHandShakeStatus::read(const Network::SocketPtr& sc) {
                LOG_ENTER;

                if (0 == hand_shake_count_) {
                    return handleShake1(sc);
                }
                else if ( 1 == hand_shake_count_ ) {
                    return handleShake2(sc);
                }
                return false;
            }

            int RtmpHandShakeStatus::write(const Network::SocketPtr& sc) {
                if (0 == hand_shake_count_) {
                    return handleShake1Write(sc);
                }
                else if (1 == hand_shake_count_) {
                    return handleShake2Write(sc);
                }
                return -1;
            }

            RtmpStatus::RtmpStatusPtr RtmpHandShakeStatus::getNext() {
                if (3 == hand_shake_count_) {
                    return nullptr;
                }
                return shared_from_this();
            }

            int RtmpHandShakeStatus::handleShake1Write(const Network::SocketPtr& sc) { 
                char* clientsig = clientbuf_ + 1;

                clientbuf_[0] = 0x03;

                unsigned int uptime = htonl(Utils::getTime());
                memcpy(clientsig, &uptime, 4);
#ifdef _DEBUG
                for (int i = 8; i < RTMP_SIG_SIZE; i++)
                    clientsig[i] = char(0xff);
#else
                for (int i = 8; i < RTMP_SIG_SIZE; i++)
                    clientsig[i] = (char)(rand() % 256);
#endif
                return sc->write(clientbuf_, RTMP_SIG_SIZE + 1);
            }

            int RtmpHandShakeStatus::handleShake2Write(const Network::SocketPtr& sc) {
                LOG_ENTER;

                unsigned int suptime = htonl(Utils::getTime());
                memcpy(&suptime, serversig_, 4);
                suptime = ntohl(suptime);

                LOGI("Server Uptime={}", suptime);
                LOGI("FMS Version= {}.{}.{}.{}",serversig_[4], serversig_[5], serversig_[6], serversig_[7]);

                return sc->write(serversig_, RTMP_SIG_SIZE);
            }

            bool RtmpHandShakeStatus::handleShake1(const Network::SocketPtr& sc) {
                LOG_ENTER;

                char type;
                int ret = sc->read(&type, sizeof(char));
                if (1 != ret) {/* 0x03 or 0x06 */
                    LOGE("read type failed ret={}", ret);
                    return false;
                }

                if (type != clientbuf_[0]) {
                    LOGD("client type={} server type={}", clientbuf_[0], type);
                }
                
                ret = sc->read(serversig_, sizeof(char) * RTMP_SIG_SIZE);
                if (RTMP_SIG_SIZE != ret) {
                    LOGE("read server sig failed len={}", ret);
                    return false;
                }

                hand_shake_count_ = 1;

                write(sc);
                
                return true;
            }

            bool RtmpHandShakeStatus::handleShake2(const Network::SocketPtr& sc) {
                if (sc->read(serversig_, RTMP_SIG_SIZE) != RTMP_SIG_SIZE)
                    return false;

                char* client_buf = clientbuf_ + 1;
                if (memcmp(serversig_, client_buf, RTMP_SIG_SIZE) == 0) {
                    LOGI("client signature does not match!");
                }

                hand_shake_count_ = 3;
                return true;
            }

        }
    }
}