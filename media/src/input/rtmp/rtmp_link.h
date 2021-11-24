#ifndef src_input_rtmp_rtmp_link_h_
#define src_input_rtmp_rtmp_link_h_

#include <memory>
#include <string>

#include "src/lj_defined.h"
#include "src/input/rtmp/rtmp_define.h"

namespace LJMP {
    namespace Input {
        namespace Rtmp {

            class RtmpLink : public std::enable_shared_from_this<RtmpLink> {
                disable_copy(RtmpLink)

            public:
                using Ptr = std::shared_ptr<RtmpLink>;
                using WPts = std::weak_ptr<RtmpLink>;

                static Ptr create();

            public:
                struct Link {
                    AVal host_name_;
                    AVal socks_host_;

                    AVal playpath0_;	/* parsed from URL */
                    AVal playpath_;	/* passed in explicitly */
                    AVal tc_url_;
                    AVal swf_url_;
                    AVal page_url_;
                    AVal app_;
                    AVal auth_;
                    AVal flash_ver_;
                    AVal subscribe_path_;
                    AVal usher_token_;
                    AVal token_;
                    AVal pub_user_;
                    AVal pub_passwd_;
                    AMFObject extras_;
                    int edepth_;

                    int seek_time_;
                    int stop_time_;

#define RTMP_LF_AUTH	0x0001	/* using auth param */
#define RTMP_LF_LIVE	0x0002	/* stream is live */
#define RTMP_LF_SWFV	0x0004	/* do SWF verification */
#define RTMP_LF_PLST	0x0008	/* send playlist before play */
#define RTMP_LF_BUFX	0x0010	/* toggle stream on BufferEmpty msg */
#define RTMP_LF_FTCU	0x0020	/* free tcUrl on close */
#define RTMP_LF_FAPU	0x0040	/* free app on close */
                    int lflags_;

                    int swf_age_;

                    int protocol_;
                    int timeout_;		/* connection timeout in seconds */

                    int pflags_;			/* unused, but kept to avoid breaking ABI */

                    unsigned short socks_port_;
                    unsigned short port_;

#ifdef CRYPTO
#define RTMP_SWF_HASHLEN	32
                    void* dh;			/* for encryption */
                    void* rc4keyIn;
                    void* rc4keyOut;

                    uint32_t SWFSize;
                    uint8_t SWFHash[RTMP_SWF_HASHLEN];
                    char SWFVerificationResponse[RTMP_SWF_HASHLEN + 10];
#endif
                };

            public:
                virtual ~RtmpLink();

                bool parseUrl(const std::string& url);

            protected:
                RtmpLink();

            private:
                bool parseURL(const char* url, int* protocol, AVal* host,
                    unsigned int* port, AVal* playpath, AVal* app);
                void parsePlaypath(AVal* in, AVal* out);
                void socksSetup(AVal* sockshost);
                bool SetOpt(const AVal* opt, AVal* arg);
                bool parseAMF(AMFObject* obj, AVal* av, int* depth);
                void optUsage();

            public:
                Link link_;
                friend struct urlopt;
            };
        }
    }
}

#endif // !src_input_rtmp_rtmp_link_h_

