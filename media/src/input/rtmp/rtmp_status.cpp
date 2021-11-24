#include "src/input/rtmp/rtmp_status.h"

#include "src/log.h"
#include "src/utils.h"
#include "src/network/socket.h"
#include "src/input/rtmp/rtmp_context.h"
#include "src/input/rtmp/rtmp_packet.h"
#include "src/input/rtmp/rtmp_utils.h"

namespace LJMP {
    namespace Input {
        namespace Rtmp {

#define SAVC(x)	static const AVal av_##x = AVC(#x)

            SAVC(app);
            SAVC(connect);
            SAVC(flashVer);
            SAVC(swfUrl);
            SAVC(pageUrl);
            SAVC(tcUrl);
            SAVC(fpad);
            SAVC(capabilities);
            SAVC(audioCodecs);
            SAVC(videoCodecs);
            SAVC(videoFunction);
            SAVC(objectEncoding);
            SAVC(secureToken);
            SAVC(secureTokenResponse);
            SAVC(type);
            SAVC(nonprivate);

            const char kRtmpStatusUnknown[] = "unknown";
            const char kRtmpStatusHandShake[] = "handShake";
            const char kRtmpStatusConnecting[] = "connect";

            RtmpStatus::RtmpStatus(std::weak_ptr<RtmpContext> rtmp_context, Network::Channel::Ptr channel)
                : rtmp_context_(rtmp_context)
                , channel_(channel) {
                LOGI("actor {}", (long long)this);

            }

            RtmpStatus::~RtmpStatus() {
                LOGI("dctor {}", (long long)this);
            }

            void RtmpStatus::readCallback(const Network::Channel::Ptr& channel, RtmpStatusWPtr wThis) {
                Ptr self(wThis.lock());
                if (!self) {
                    LOGW("this object is destruct this={}", (long long)this);
                    return;
                }

                if (getChannel() != channel) {
                    LOGW("channel is not equal");
                    return;
                }

                read();
            }

            void RtmpStatus::switchToSelf() {
                write();
            }

            bool RtmpStatus::writeInChannel(const DataBuffer::Ptr& buffer) {
                Network::Channel::Ptr channel(channel_.lock());
                if (!channel) {
                    LOGW("this object is destruct this={}", (long long)this);
                    return false;
                }

                RtmpStatusWPtr wThis(shared_from_this());
                return channel->write(buffer, std::bind(&RtmpStatus::writeStatusCallback, this,
                    std::placeholders::_1, wThis));
            }

            void RtmpStatus::writeStatusCallback(bool status, RtmpStatusWPtr wThis) {
                Ptr self(wThis.lock());
                if (!self) {
                    LOGW("this object is destruct this={}", (long long)this);
                    return;
                }

                write_failed_ = status;
            }

            RtmpStatus::Ptr RtmpHandShakeStatus::create(RtmpContext::WPtr rtmp_context,
                Network::Channel::Ptr channel) {
                struct Creator : public RtmpHandShakeStatus {
                    explicit Creator(RtmpContext::WPtr rtmp_context,
                        Network::Channel::Ptr channel) : RtmpHandShakeStatus(rtmp_context, channel) {
                    }
                    ~Creator() override = default;
                };

                Ptr self = std::make_shared<Creator>(rtmp_context, channel);
                if (channel) {
                    RtmpStatusWPtr wSelf(self);
                    channel->setReadCallbackHandle(std::bind(&RtmpStatus::readCallback, self.get(),
                        std::placeholders::_1, wSelf));
                }

                return self;
            }

            RtmpHandShakeStatus::RtmpHandShakeStatus(RtmpContext::WPtr rtmp_contxt,
                Network::Channel::Ptr channel)
                : RtmpStatus(rtmp_contxt, channel) {

            }

            bool RtmpHandShakeStatus::read() {
                LOG_ENTER;

                if (0 == hand_shake_count_) {
                    return handleShake1();
                }
                else if (1 == hand_shake_count_) {
                    return handleShake2();
                }
                return false;
            }

            int RtmpHandShakeStatus::write() {
                if (0 == hand_shake_count_) {
                    return handleShake1Write();
                }
                else if (1 == hand_shake_count_) {
                    return handleShake2Write();
                }
                return -1;
            }

            int RtmpHandShakeStatus::handleShake1Write() {
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
                DataBuffer::Ptr data_buffer = DataBuffer::create(clientbuf_, RTMP_SIG_SIZE + 1);
                writeInChannel(data_buffer);
                return 1;
            }

            int RtmpHandShakeStatus::handleShake2Write() {
                LOG_ENTER;

                unsigned int suptime = htonl(Utils::getTime());
                memcpy(&suptime, serversig_, 4);
                suptime = ntohl(suptime);

                LOGI("Server Uptime={}", suptime);
                LOGI("FMS Version= {}.{}.{}.{}", serversig_[4], serversig_[5], serversig_[6], serversig_[7]);

                DataBuffer::Ptr data_buffer = DataBuffer::create(serversig_, RTMP_SIG_SIZE);
                writeInChannel(data_buffer);
                return 1;
            }

            bool RtmpHandShakeStatus::handleShake1() {
                LOG_ENTER;

                Network::Channel::Ptr channel = getChannel();
                if (!channel) {
                    LOGE("channel is desturct");
                    return false;
                }
                DataBuffer::Ptr type_buffer = DataBuffer::create(1);
                int ret = channel->read(type_buffer);
                if (1 != ret) {/* 0x03 or 0x06 */
                    LOGE("read type failed ret={}", ret);
                    return false;
                }

                if (type_buffer->getData()[0] != clientbuf_[0]) {
                    LOGD("client type={} server type={}", clientbuf_[0], type_buffer->getData()[0]);
                }

                DataBuffer::Ptr data_buffer = DataBuffer::create(sizeof(char) * RTMP_SIG_SIZE);
                ret = channel->read(data_buffer);
                if (RTMP_SIG_SIZE != ret) {
                    LOGE("read server sig failed len={}", ret);
                    return false;
                }

                memcpy(serversig_, data_buffer->getData(), data_buffer->getSize());
                hand_shake_count_ = 1;

                write();

                return true;
            }

            bool RtmpHandShakeStatus::handleShake2() {
                Network::Channel::Ptr channel = getChannel();
                if (!channel) {
                    LOGE("channel is desturct");
                    return false;
                }

                DataBuffer::Ptr data_buffer = DataBuffer::create(sizeof(char) * RTMP_SIG_SIZE);
                int ret = channel->read(data_buffer);
                if (ret != RTMP_SIG_SIZE)
                    return false;

                char* client_buf = clientbuf_ + 1;
                if (memcmp(serversig_, client_buf, RTMP_SIG_SIZE) != 0) {
                    LOGI("client signature does not match!");
                }

                hand_shake_count_ = 3;
                RtmpContext::Ptr rtmp_context = getRtmpContext();
                if (!rtmp_context) {
                    LOGE("rtmp context is destruct");
                    return false;
                }

                RtmpStatus::Ptr conntct_status = RtmpConnectStatus::create(rtmp_context, channel);
                rtmp_context->switchStatus(conntct_status);
                return true;
            }

            RtmpStatus::Ptr RtmpConnectStatus::create(std::weak_ptr<RtmpContext> rtmp_context,
                Network::Channel::Ptr channel) {
                struct Creator : public RtmpConnectStatus {
                    explicit Creator(std::weak_ptr<RtmpContext> rtmp_context,
                        Network::Channel::Ptr channel) : RtmpConnectStatus(rtmp_context, channel) {
                    }
                    ~Creator() override = default;
                };
                return std::make_shared<Creator>(rtmp_context, channel);
            }

            RtmpConnectStatus::RtmpConnectStatus(std::weak_ptr<RtmpContext> rtmp_context,
                Network::Channel::Ptr channel)
                : RtmpStatus(rtmp_context, channel) {

            }

            RtmpConnectStatus::~RtmpConnectStatus() {

            }

            bool RtmpConnectStatus::read() {
                return false;
            }

            int RtmpConnectStatus::write() {
                LOG_ENTER;

                RtmpContext::Ptr rtmp_contxt = getRtmpContext();
                if (!rtmp_contxt) {
                    LOGE("rtmp context is nullptr");
                    return -1;
                }

                DataBuffer::Ptr data_buffer = DataBuffer::create(4096);


                RtmpPacket::Ptr packet = RtmpPacket::create();
                packet->setChannel(0x3);
                packet->setPacketType(RTMP_PACKET_TYPE_INVOKE);

                packet->setBody(data_buffer);
                data_buffer->setOffset(RTMP_MAX_HEADER_SIZE);

                char* enc = data_buffer->getOffsetData();
                char *pend = data_buffer->getData() + data_buffer->getSize();

                enc = RtmpUtils::amfEncodeString(enc, pend, &av_connect);
                enc = RtmpUtils::amfEncodeNumber(enc, pend, ++rtmp_contxt->numInvokes());
                *enc++ = AMF_OBJECT;

                enc = RtmpUtils::amfEncodeNamedString(enc, pend, &av_app, rtmp_contxt->getAppName());
                if (!enc) {
                    return 1;
                }
                    
                if (rtmp_contxt->getProtocol() & RTMP_FEATURE_WRITE) {
                    enc = RtmpUtils::amfEncodeNamedString(enc, pend, &av_type, &av_nonprivate);
                    if (!enc)
                        return FALSE;
                }
#if 0
                if (rtmp_contxt->getAppName().flashVer.av_len) {
                    enc = RtmpUtils::amfEncodeNamedString(enc, pend, &av_flashVer, &r->Link.flashVer);
                    if (!enc)
                        return FALSE;
                }

                if (r->Link.swfUrl.av_len) {
                    enc = AMF_EncodeNamedString(enc, pend, &av_swfUrl, &r->Link.swfUrl);
                    if (!enc)
                        return FALSE;
                }
                if (r->Link.tcUrl.av_len) {
                    enc = AMF_EncodeNamedString(enc, pend, &av_tcUrl, &r->Link.tcUrl);
                    if (!enc)
                        return FALSE;
                }
                if (!(r->Link.protocol & RTMP_FEATURE_WRITE)) {
                    enc = AMF_EncodeNamedBoolean(enc, pend, &av_fpad, FALSE);
                    if (!enc)
                        return FALSE;
                    enc = AMF_EncodeNamedNumber(enc, pend, &av_capabilities, 15.0);
                    if (!enc)
                        return FALSE;
                    enc = AMF_EncodeNamedNumber(enc, pend, &av_audioCodecs, r->m_fAudioCodecs);
                    if (!enc)
                        return FALSE;
                    enc = AMF_EncodeNamedNumber(enc, pend, &av_videoCodecs, r->m_fVideoCodecs);
                    if (!enc)
                        return FALSE;
                    enc = AMF_EncodeNamedNumber(enc, pend, &av_videoFunction, 1.0);
                    if (!enc)
                        return FALSE;
                    if (r->Link.pageUrl.av_len) {
                        enc = AMF_EncodeNamedString(enc, pend, &av_pageUrl, &r->Link.pageUrl);
                        if (!enc)
                            return FALSE;
                    }
                }
                if (r->m_fEncoding != 0.0 || r->m_bSendEncoding) {	/* AMF0, AMF3 not fully supported yet */
                    enc = AMF_EncodeNamedNumber(enc, pend, &av_objectEncoding, r->m_fEncoding);
                    if (!enc)
                        return FALSE;
                }
                if (enc + 3 >= pend)
                    return FALSE;
                *enc++ = 0;
                *enc++ = 0;			/* end of object - 0x00 0x00 0x09 */
                *enc++ = AMF_OBJECT_END;

                /* add auth string */
                if (r->Link.auth.av_len) {
                    enc = AMF_EncodeBoolean(enc, pend, r->Link.lFlags & RTMP_LF_AUTH);
                    if (!enc)
                        return FALSE;
                    enc = AMF_EncodeString(enc, pend, &r->Link.auth);
                    if (!enc)
                        return FALSE;
                }
                if (r->Link.extras.o_num) {
                    int i;
                    for (i = 0; i < r->Link.extras.o_num; i++) {
                        enc = AMFProp_Encode(&r->Link.extras.o_props[i], enc, pend);
                        if (!enc)
                            return FALSE;
                    }
                }
                packet.m_nBodySize = enc - packet.m_body;
#endif 
                return true;
            }
        }
    }
}