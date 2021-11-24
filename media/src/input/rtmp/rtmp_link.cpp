#include "src/input/rtmp/rtmp_link.h"

#include "src/log.h"
#include "src/input/rtmp/rtmp_utils.h"
#include "src/input/rtmp/rtmp_define.h"

#ifdef _MSC_VER
#define strcasecmp _stricmp
#define strncasecmp _strnicmp
#endif

namespace LJMP {
    namespace Input {
        namespace Rtmp {
            const char RTMPProtocolStrings[][7] = {
              "RTMP",
              "RTMPT",
              "RTMPE",
              "RTMPTE",
              "RTMPS",
              "RTMPTS",
              "",
              "",
              "RTMFP"
            };

            const char RTMPProtocolStringsLower[][7] = {
              "rtmp",
              "rtmpt",
              "rtmpe",
              "rtmpte",
              "rtmps",
              "rtmpts",
              "",
              "",
              "rtmfp"
            };

            enum { OPT_STR = 0, OPT_INT, OPT_BOOL, OPT_CONN };
            static const char* optinfo[] = { "string", "integer", "boolean", "AMF" };
            static const AVal truth[] = {
                AVC("1"),
                AVC("on"),
                AVC("yes"),
                AVC("true"),
                {0,0}
            };

#undef OSS
#ifdef _WIN32
#define OSS	"WIN"
#elif defined(__sun__)
#define OSS	"SOL"
#elif defined(__APPLE__)
#define OSS	"MAC"
#elif defined(__linux__)
#define OSS	"LNX"
#else
#define OSS	"GNU"
#endif
#define DEF_VERSTR	OSS " 10,0,32,18"
#define OFF(x)	offsetof(class RtmpLink,x)

            static struct urlopt {
                AVal name;
                off_t off;
                int otype;
                int omisc;
                char* use;
            } options[] = {
              { AVC("socks"),     OFF(link_.socks_host_),     OPT_STR, 0,
                "Use the specified SOCKS proxy" },
              { AVC("app"),       OFF(link_.app_),           OPT_STR, 0,
                "Name of target app on server" },
              { AVC("tcUrl"),     OFF(link_.tc_url_),         OPT_STR, 0,
                "URL to played stream" },
              { AVC("pageUrl"),   OFF(link_.page_url_),       OPT_STR, 0,
                "URL of played media's web page" },
              { AVC("swfUrl"),    OFF(link_.swf_url_),        OPT_STR, 0,
                "URL to player SWF file" },
              { AVC("flashver"),  OFF(link_.flash_ver_),      OPT_STR, 0,
                "Flash version string (default " DEF_VERSTR ")" },
              { AVC("conn"),      OFF(link_.extras_),        OPT_CONN, 0,
                "Append arbitrary AMF data to Connect message" },
              { AVC("playpath"),  OFF(link_.playpath_),      OPT_STR, 0,
                "Path to target media on server" },
              { AVC("playlist"),  OFF(link_.lflags_),        OPT_BOOL, RTMP_LF_PLST,
                "Set playlist before play command" },
              { AVC("live"),      OFF(link_.lflags_),        OPT_BOOL, RTMP_LF_LIVE,
                "Stream is live, no seeking possible" },
              { AVC("subscribe"), OFF(link_.subscribe_path_), OPT_STR, 0,
                "Stream to subscribe to" },
              { AVC("jtv"), OFF(link_.usher_token_),          OPT_STR, 0,
                "Justin.tv authentication token" },
              { AVC("token"),     OFF(link_.token_),	       OPT_STR, 0,
                "Key for SecureToken response" },
              { AVC("swfVfy"),    OFF(link_.lflags_),        OPT_BOOL, RTMP_LF_SWFV,
                "Perform SWF Verification" },
              { AVC("swfAge"),    OFF(link_.swf_age_),        OPT_INT, 0,
                "Number of days to use cached SWF hash" },
              { AVC("start"),     OFF(link_.seek_time_),      OPT_INT, 0,
                "Stream start position in milliseconds" },
              { AVC("stop"),      OFF(link_.stop_time_),      OPT_INT, 0,
                "Stream stop position in milliseconds" },
              //{ AVC("buffer"),    OFF(m_nBufferMS),        OPT_INT, 0,
              //  "Buffer time in milliseconds" },
              { AVC("timeout"),   OFF(link_.timeout_),       OPT_INT, 0,
                "Session timeout in seconds" },
              { AVC("pubUser"),   OFF(link_.pub_user_),       OPT_STR, 0,
                    "Publisher username" },
              { AVC("pubPasswd"), OFF(link_.pub_passwd_),     OPT_STR, 0,
                    "Publisher password" },
              { {NULL,0}, 0, 0}
            };

            RtmpLink::Ptr RtmpLink::create() {
                struct Creator : public RtmpLink {
                    Creator() : RtmpLink() {}
                    ~Creator() override = default;
                };
                return std::make_shared<Creator>();
            }

            RtmpLink::RtmpLink() {
                LOGI("actor {}", (long long)this);
                memset(&link_, 0, sizeof(link_));

            }

            RtmpLink::~RtmpLink() {
                LOGI("dactor {}", (long long)this);
            }

            bool RtmpLink::parseUrl(const std::string& url) {
                LOG_ENTER;

                AVal opt, arg;
                char* p1, * p2;
                char *ptr = const_cast<char*>(strchr(url.c_str(), ' '));
                unsigned int port = 0;

                int len = static_cast<int>(url.length());
                bool ret = doParseURL(url.c_str(), &link_.protocol_, &link_.host_name_,
                    &port, &link_.playpath0_, &link_.app_);
                if (!ret) {
                    return ret;
                }

                link_.port_ = port;
                link_.playpath_ = link_.playpath0_;

                while (ptr) {
                    *ptr++ = '\0';
                    p1 = ptr;
                    p2 = strchr(p1, '=');
                    if (!p2)
                        break;
                    opt.av_val = p1;
                    opt.av_len = static_cast<int>(p2 - p1);
                    *p2++ = '\0';
                    arg.av_val = p2;
                    ptr = strchr(p2, ' ');
                    if (ptr) {
                        *ptr = '\0';
                        arg.av_len = static_cast<int>(ptr - p2);
                        /* skip repeated spaces */
                        while (ptr[1] == ' ')
                            *ptr++ = '\0';
                    }
                    else {
                        arg.av_len = static_cast<int>(strlen(p2));
                    }

                    /* unescape */
                    port = arg.av_len;
                    for (p1 = p2; port > 0;) {
                        if (*p1 == '\\') {
                            unsigned int c;
                            if (port < 3)
                                return FALSE;
                            sscanf(p1 + 1, "%02x", &c);
                            *p2++ = c;
                            port -= 3;
                            p1 += 3;
                        }
                        else {
                            *p2++ = *p1++;
                            port--;
                        }
                    }
                    arg.av_len = static_cast<int>(p2 - arg.av_val);

                    ret = setOpt(&opt, &arg);
                    if (!ret)
                        return ret;
                }

                if (!link_.tc_url_.av_len) {
                    link_.tc_url_.av_val = const_cast<char*>(url.c_str());
                    if (link_.app_.av_len) {
                        if (link_.app_.av_val < url.c_str() + len) {
                            /* if app is part of original url, just use it */
                            link_.tc_url_.av_len = link_.app_.av_len
                                + static_cast<int>((link_.app_.av_val - url.c_str()));
                        }
                        else {
                            len = link_.host_name_.av_len + link_.app_.av_len +
                                sizeof("rtmpte://:65535/");
                            link_.tc_url_.av_val = (char*)malloc(len);
                            link_.tc_url_.av_len = snprintf(link_.tc_url_.av_val, len,
                                "%s://%.*s:%d/%.*s",
                                RTMPProtocolStringsLower[link_.protocol_],
                                link_.host_name_.av_len, link_.host_name_.av_val,
                                link_.port_,
                                link_.app_.av_len, link_.app_.av_val);
                            link_.lflags_ |= RTMP_LF_FTCU;
                        }
                    }
                    else {
                        link_.tc_url_.av_len = static_cast<int>(url.length());
                    }
                }

#ifdef CRYPTO
                if ((r->Link.lFlags & RTMP_LF_SWFV) && r->Link.swfUrl.av_len)
                    RTMP_HashSWF(r->Link.swfUrl.av_val, &r->Link.SWFSize,
                        (unsigned char*)r->Link.SWFHash, r->Link.swfAge);
#endif

                socksSetup(&link_.socks_host_);

                if (link_.port_ == 0) {
                    if (link_.protocol_ & RTMP_FEATURE_SSL)
                        link_.port_ = 443;
                    else if (link_.protocol_ & RTMP_FEATURE_HTTP)
                        link_.port_ = 80;
                    else
                        link_.port_ = 1935;
                }
                return true;
            }

            bool RtmpLink::doParseURL(const char* url, int* protocol, AVal* host, unsigned int* port,
                AVal* playpath, AVal* app) {
                char* p, * end, * col, * ques, * slash;

                LOGI("Parsing...");

                *protocol = RTMP_PROTOCOL_RTMP;
                *port = 0;
                playpath->av_len = 0;
                playpath->av_val = NULL;
                app->av_len = 0;
                app->av_val = NULL;

                /* Old School Parsing */

                /* look for usual :// pattern */
                p = (char*)strstr(url, "://");
                if (!p) {
                    LOGE("RTMP URL: No :// in url!");
                    return false;
                }
                {
                    int len = (int)(p - url);

                    if (len == 4 && strncasecmp(url, kRtmp, 4) == 0)
                        *protocol = RTMP_PROTOCOL_RTMP;
                    else if (len == 5 && strncasecmp(url, kRtmpt, 5) == 0)
                        *protocol = RTMP_PROTOCOL_RTMPT;
                    else if (len == 5 && strncasecmp(url, kRtmps, 5) == 0)
                        *protocol = RTMP_PROTOCOL_RTMPS;
                    else if (len == 5 && strncasecmp(url, kRtmpe, 5) == 0)
                        *protocol = RTMP_PROTOCOL_RTMPE;
                    else if (len == 5 && strncasecmp(url, kRtmfp, 5) == 0)
                        *protocol = RTMP_PROTOCOL_RTMFP;
                    else if (len == 6 && strncasecmp(url, kRtmpte, 6) == 0)
                        *protocol = RTMP_PROTOCOL_RTMPTE;
                    else if (len == 6 && strncasecmp(url, kRtmpts, 6) == 0)
                        *protocol = RTMP_PROTOCOL_RTMPTS;
                    else {
                        LOGW("Unknown protocol!");
                        goto parsehost;
                    }
                }

                LOGI("Parsed protocol: {}", *protocol);

            parsehost:
                /* let's get the hostname */
                p += 3;

                /* check for sudden death */
                if (*p == 0) {
                    LOGE( "No hostname in URL!");
                    return FALSE;
                }

                end = p + strlen(p);
                col = strchr(p, ':');
                ques = strchr(p, '?');
                slash = strchr(p, '/');

                {
                    int hostlen;
                    if (slash)
                        hostlen = static_cast<int>(slash - p);
                    else
                        hostlen = static_cast<int>(end - p);
                    if (col && col - p < hostlen)
                        hostlen = static_cast<int>(col - p);

                    if (hostlen < 256) {
                        host->av_val = p;
                        host->av_len = hostlen;
                        LOGD("Parsed host    : {},{}", hostlen, host->av_val);
                    }
                    else {
                        LOGW("Hostname exceeds 255 characters!");
                    }

                    p += hostlen;
                }

                /* get the port number if available */
                if (*p == ':') {
                    unsigned int p2;
                    p++;
                    p2 = atoi(p);
                    if (p2 > 65535) {
                        LOGE("Invalid port number!");
                    }
                    else {
                        *port = p2;
                    }
                }

                if (!slash) {
                    LOGW("No application or playpath in URL!");
                    return true;
                }
                p = slash + 1;

                {
                    /* parse application
                     *
                     * rtmp://host[:port]/app[/appinstance][/...]
                     * application = app[/appinstance]
                     */

                    char* slash2, * slash3 = NULL, * slash4 = NULL;
                    int applen, appnamelen;

                    slash2 = strchr(p, '/');
                    if (slash2)
                        slash3 = strchr(slash2 + 1, '/');
                    if (slash3)
                        slash4 = strchr(slash3 + 1, '/');

                    applen = static_cast<int>(end - p); /* ondemand, pass all parameters as app */
                    appnamelen = applen; /* ondemand length */

                    if (ques && strstr(p, "slist=")) { /* whatever it is, the '?' and slist= means we need to use everything as app and parse plapath from slist= */
                        appnamelen = static_cast<int>(ques - p);
                    }
                    else if (strncmp(p, "ondemand/", 9) == 0) {
                        /* app = ondemand/foobar, only pass app=ondemand */
                        applen = 8;
                        appnamelen = 8;
                    }
                    else { /* app!=ondemand, so app is app[/appinstance] */
                        if (slash4)
                            appnamelen = static_cast<int>(slash4 - p);
                        else if (slash3)
                            appnamelen = static_cast<int>(slash3 - p);
                        else if (slash2)
                            appnamelen = static_cast<int>(slash2 - p);

                        applen = appnamelen;
                    }

                    app->av_val = p;
                    app->av_len = applen;
                    LOGD("Parsed app     : {}.{}", applen, p);

                    p += appnamelen;
                }

                if (*p == '/')
                    p++;

                if (end - p) {
                    AVal av = { p, static_cast<int>(end - p) };
                    parsePlaypath(&av, playpath);
                }

                return true;
            }

            void RtmpLink::parsePlaypath(AVal* in, AVal* out) {
                int addMP4 = 0;
                int addMP3 = 0;
                int subExt = 0;
                const char* playpath = in->av_val;
                const char* temp, * q, * ext = NULL;
                const char* ppstart = playpath;
                char* streamname, * destptr, * p;

                int pplen = in->av_len;

                out->av_val = NULL;
                out->av_len = 0;

                if ((*ppstart == '?') &&
                    (temp = strstr(ppstart, "slist=")) != 0) {
                    ppstart = temp + 6;
                    pplen = static_cast<int>(strlen(ppstart));

                    temp = strchr(ppstart, '&');
                    if (temp) {
                        pplen = static_cast<int>(temp - ppstart);
                    }
                }

                q = strchr(ppstart, '?');
                if (pplen >= 4) {
                    if (q)
                        ext = q - 4;
                    else
                        ext = &ppstart[pplen - 4];
                    if ((strncmp(ext, ".f4v", 4) == 0) ||
                        (strncmp(ext, ".mp4", 4) == 0)) {
                        addMP4 = 1;
                        subExt = 1;
                        /* Only remove .flv from rtmp URL, not slist params */
                    }
                    else if ((ppstart == playpath) &&
                        (strncmp(ext, ".flv", 4) == 0)) {
                        subExt = 1;
                    }
                    else if (strncmp(ext, ".mp3", 4) == 0) {
                        addMP3 = 1;
                        subExt = 1;
                    }
                }

                streamname = (char*)malloc((pplen + 4 + 1) * sizeof(char));
                if (!streamname)
                    return;

                destptr = streamname;
                if (addMP4) {
                    if (strncmp(ppstart, "mp4:", 4)) {
                        strcpy(destptr, "mp4:");
                        destptr += 4;
                    }
                    else {
                        subExt = 0;
                    }
                }
                else if (addMP3) {
                    if (strncmp(ppstart, "mp3:", 4)) {
                        strcpy(destptr, "mp3:");
                        destptr += 4;
                    }
                    else {
                        subExt = 0;
                    }
                }

                for (p = (char*)ppstart; pplen > 0;) {
                    /* skip extension */
                    if (subExt && p == ext) {
                        p += 4;
                        pplen -= 4;
                        continue;
                    }
                    if (*p == '%') {
                        unsigned int c;
                        sscanf(p + 1, "%02x", &c);
                        *destptr++ = c;
                        pplen -= 3;
                        p += 3;
                    }
                    else {
                        *destptr++ = *p++;
                        pplen--;
                    }
                }
                *destptr = '\0';

                out->av_val = streamname;
                out->av_len = static_cast<int>(destptr - streamname);
            }

            void RtmpLink::socksSetup(AVal* sockshost) {
                if (sockshost->av_len) {
                    const char* socksport = strchr(sockshost->av_val, ':');
                    char* hostname = strdup(sockshost->av_val);

                    if (socksport) {
                        hostname[socksport - sockshost->av_val] = '\0';
                    }
                        
                    link_.socks_host_.av_val = hostname;
                    link_.socks_host_.av_len = static_cast<int>(strlen(hostname));

                    link_.socks_port_ = socksport ? atoi(socksport + 1) : 1080;
                    LOGD("Connecting via SOCKS proxy: {}:{}",link_.socks_host_.av_val, link_.socks_port_);
                }
                else {
                    link_.socks_host_.av_val = NULL;
                    link_.socks_host_.av_len = 0;
                    link_.socks_port_ = 0;
                }
            }

            bool RtmpLink::setOpt(const AVal* opt, AVal* arg) {
                int i;
                void* v;

                for (i = 0; options[i].name.av_len; i++) {
                    if (opt->av_len != options[i].name.av_len) continue;
                    if (strcasecmp(opt->av_val, options[i].name.av_val)) continue;
                    v = (char*)(&link_) + options[i].off;
                    switch (options[i].otype) {
                    case OPT_STR: {
                        AVal* aptr = (AVal*)v;
                        *aptr = *arg; }
                                break;
                    case OPT_INT: {
                        long l = strtol(arg->av_val, NULL, 0);
                        *(int*)v = l; }
                                break;
                    case OPT_BOOL: {
                        int j, fl;
                        fl = *(int*)v;
                        for (j = 0; truth[j].av_len; j++) {
                            if (arg->av_len != truth[j].av_len) continue;
                            if (strcasecmp(arg->av_val, truth[j].av_val)) continue;
                            fl |= options[i].omisc; break;
                        }
                        *(int*)v = fl;
                    }
                                 break;
                    case OPT_CONN:
                        if (parseAMF(&link_.extras_, arg, &link_.edepth_))
                            return false;
                        break;
                    }
                    break;
                }
                if (!options[i].name.av_len) {
                    LOGE("Unknown option {}", opt->av_val);
                    optUsage();
                    return false;
                }

                return true;
            }

            bool RtmpLink::parseAMF(AMFObject* obj, AVal* av, int* depth) {
                AMFObjectProperty prop = { {0,0} };
                int i;
                char* p, * arg = av->av_val;

                if (arg[1] == ':') {
                    p = (char*)arg + 2;
                    switch (arg[0]) {
                    case 'B':
                        prop.p_type = AMF_BOOLEAN;
                        prop.p_vu.p_number = atoi(p);
                        break;
                    case 'S':
                        prop.p_type = AMF_STRING;
                        prop.p_vu.p_aval.av_val = p;
                        prop.p_vu.p_aval.av_len = av->av_len - static_cast<int>((p - arg));
                        break;
                    case 'N':
                        prop.p_type = AMF_NUMBER;
                        prop.p_vu.p_number = strtod(p, NULL);
                        break;
                    case 'Z':
                        prop.p_type = AMF_NULL;
                        break;
                    case 'O':
                        i = atoi(p);
                        if (i) {
                            prop.p_type = AMF_OBJECT;
                        }
                        else {
                            (*depth)--;
                            return true;
                        }
                        break;
                    default:
                        return false;
                    }
                }
                else if (arg[2] == ':' && arg[0] == 'N') {
                    p = strchr(arg + 3, ':');
                    if (!p || !*depth) {
                        return false;
                    }

                    prop.p_name.av_val = (char*)arg + 3;
                    prop.p_name.av_len = static_cast<int>(p - (arg + 3));

                    p++;
                    switch (arg[1]) {
                    case 'B':
                        prop.p_type = AMF_BOOLEAN;
                        prop.p_vu.p_number = atoi(p);
                        break;
                    case 'S':
                        prop.p_type = AMF_STRING;
                        prop.p_vu.p_aval.av_val = p;
                        prop.p_vu.p_aval.av_len = av->av_len - static_cast<int>((p - arg));
                        break;
                    case 'N':
                        prop.p_type = AMF_NUMBER;
                        prop.p_vu.p_number = strtod(p, NULL);
                        break;
                    case 'O':
                        prop.p_type = AMF_OBJECT;
                        break;
                    default:
                        return false;
                    }
                }
                else {
                    return false;
                }
                   

                if (*depth) {
                    AMFObject* o2;
                    for (i = 0; i < *depth; i++) {
                        o2 = &obj->o_props[obj->o_num - 1].p_vu.p_object;
                        obj = o2;
                    }
                }
                RtmpUtils::amfAddProp(obj, &prop);
                if (prop.p_type == AMF_OBJECT)
                    (*depth)++;
                return 0;
            }

            void RtmpLink::optUsage() {
                LOG_ENTER;

                for (int i = 0; options[i].name.av_len; i++) {
                    LOGE("{} {} {}", options[i].name.av_val, optinfo[options[i].otype], options[i].use);
                }
            }

        }
    }
}